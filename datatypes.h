#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>

typedef unsigned char managerID_t;
typedef unsigned char controlID_t;
typedef unsigned char hardwareID_t;
typedef unsigned char protocol_t;
typedef unsigned short address_t;
typedef short speed_t;
typedef unsigned short locoID_t;

enum managerIDs : managerID_t {
  MANAGER_ID_CONSOLE = 0,
  MANAGER_ID_HARDWARE,
  MANAGER_ID_WEBSERVER
};

enum protocols : protocol_t {
	PROTOCOL_NONE = 0,
	PROTOCOL_ALL,
	PROTOCOL_MM1,
	PROTOCOL_MM2,
	PROTOCOL_MFX,
	PROTOCOL_DCC,
	PROTOCOL_SX1,
	PROTOCOL_SX2
};

enum hardwareIDs : hardwareID_t {
	HARDWARE_ID_NONE = 0,
  HARDWARE_ID_VIRT,
	HARDWARE_ID_CS2,
	HARDWARE_ID_NUM
};

#define ADDRESS_NONE 0

static std::string hardwareSymbols[] = {
	"none",
	"virtual",
	"cs2"
};

#endif // DATATYPES_H

