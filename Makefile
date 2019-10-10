
CC=gcc
CPP=g++
CCRASPI=aarch64-linux-gcc
CPPRASPI=aarch64-linux-g++

CPPFLAGS=-I. -g -O0 -Wall -Wextra -Werror -std=c++11
# -O2 does not work
CPPFLAGSAMALGAMATION=-I. -g -O0 -Wall -Wextra -Werror -std=c++11
CPPFLAGSRASPI=-I. -g -O0 -Wall -Wextra -Wno-cast-function-type -Werror -std=c++11 --sysroot=/home/teddy/buildroot-2018.11/output/host/aarch64-buildroot-linux-gnu/sysroot
LDFLAGS=-g -Wl,--export-dynamic
LIBS=-lpthread -ldl

OBJ= \
	Config.o \
	DataModel/Accessory.o \
	DataModel/Feedback.o \
	DataModel/HardwareHandle.o \
	DataModel/LayoutItem.o \
	DataModel/LockableItem.o \
	DataModel/Loco.o \
	DataModel/Object.o \
	DataModel/Relation.o \
	DataModel/Serializable.o \
	DataModel/Signal.o \
	DataModel/Street.o \
	DataModel/Switch.o \
	DataModel/Track.o \
	DelayedCall.o \
	Hardware/HardwareHandler.o \
	Logger/Logger.o \
	Logger/LoggerServer.o \
	Manager.o \
	Network/Serial.o \
	Network/TcpConnection.o \
	Network/TcpServer.o \
	RailControl.o \
	Storage/StorageHandler.o \
	Timestamp.o \
	Utils/Utils.o \
	WebServer/HtmlFullResponse.o \
	WebServer/HtmlResponse.o \
	WebServer/HtmlResponseNotFound.o \
	WebServer/HtmlResponseNotImplemented.o \
	WebServer/HtmlTag.o \
	WebServer/HtmlTagAccessory.o \
	WebServer/HtmlTagButton.o \
	WebServer/HtmlTagButtonCancel.o \
	WebServer/HtmlTagButtonCommand.o \
	WebServer/HtmlTagButtonCommandToggle.o \
	WebServer/HtmlTagButtonOK.o \
	WebServer/HtmlTagButtonPopup.o \
	WebServer/HtmlTagFeedback.o \
	WebServer/HtmlTagInput.o \
	WebServer/HtmlTagInputInteger.o \
	WebServer/HtmlTagInputSlider.o \
	WebServer/HtmlTagInputSliderLocoSpeed.o \
	WebServer/HtmlTagSelect.o \
	WebServer/HtmlTagSignal.o \
	WebServer/HtmlTagStreet.o \
	WebServer/HtmlTagSwitch.o \
	WebServer/HtmlTagTrack.o \
	WebServer/Response.o \
	WebServer/WebClient.o \
	WebServer/WebServer.o

all: $(OBJ)
	make -C Hardware
	make -C Storage
	$(CPP) $(LDFLAGS) $(OBJ) -o railcontrol $(LIBS)
	rm Timestamp.h

dist: all
	strip railcontrol
	tar cvJf railcontrol.tar.xz railcontrol Storage/*.so Hardware/*.so railcontrol.conf.dist html/*

amalgamation: Timestamp.h
	./amalgamation.bash
	$(CPP) $(CPPFLAGSAMALGAMATION) -DAMALGAMATION -c -o amalgamation.o amalgamation.cpp
	make -C Storage amalgamation
	$(CPP) -g amalgamation.o Storage/sqlite/sqlite3.o -o railcontrol $(LIBS)
	rm -f amalgamation.o
	rm -f amalgamation.cpp
	rm Timestamp.h

raspi: Timestamp.h
	./amalgamation.bash
	$(CPPRASPI) $(CPPFLAGSRASPI) -DAMALGAMATION -c -o amalgamation.o amalgamation.cpp
	make -C Storage raspi
	$(CPPRASPI) -g amalgamation.o Storage/sqlite/sqlite3.o -o railcontrol $(LIBS)
	rm -f amalgamation.o
	rm -f amalgamation.cpp
	rm Timestamp.h

sqlite-shell:
	make -C Storage/sqlite

Timestamp.o: Timestamp.cpp Timestamp.h
	$(CPP) $(CPPFLAGS) -c -o $@ $<

%.o: %.cpp *.h DataModel/*.h Hardware/HardwareHandler.h Logger/*.h Storage/StorageHandler.h Utils/*.h WebServer/*.h
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean:
	make -C Hardware clean
	make -C Storage clean
	rm -f *.o DataModel/*.o Logger/*.o Network/*.o Utils/*.o WebServer/*.o
	rm -f railcontrol

clean-sqlite-shell:
	make -C Storage/sqlite clean

test:
	make -C test

Timestamp.h:
	echo "#pragma once" > Timestamp.h
	echo "#define __COMPILE_TIME__ `date +%s`" >> Timestamp.h
	echo "time_t GetCompileTime();" >> Timestamp.h
