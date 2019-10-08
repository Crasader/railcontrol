/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2019 Dominik (Teddy) Mahrer - www.railcontrol.org

RailControl is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

RailControl is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RailControl; see the file LICENCE. If not see
<http://www.gnu.org/licenses/>.
*/

#pragma once

#include <mutex>
#include <string>
#include <termios.h>
#include <unistd.h>   //close & write;

#include "Logger/Logger.h"

namespace Network
{
	class Serial
	{
		public:
			Serial(Logger::Logger* logger,
				const std::string& tty,
				const unsigned int dataSpeed, // from termio (ex. B9600)
				const unsigned char dataBits,
				const char parity,
				const unsigned char stopBits)
			:	logger(logger),
			 	tty(tty),
			 	dataSpeed(dataSpeed),
			 	dataBits(dataBits),
			 	parity(parity),
			 	stopBits(stopBits)
			{
				Init();
			}

			~Serial()
			{
				Close();
			}

			void ReInit() { Close(); Init(); }

			bool IsConnected() const { return fileHandle != -1; }

			void ClearBuffers() { tcflush(fileHandle, TCIOFLUSH); }

			unsigned int Send(const std::string& data)
			{
				std::lock_guard<std::mutex> Guard(fileHandleMutex);
				return write(fileHandle, data.c_str(), data.length());
			}

			unsigned int Send(const unsigned char data)
			{
				std::lock_guard<std::mutex> Guard(fileHandleMutex);
				return write(fileHandle, &data, 1);
			}

			unsigned int Send(const unsigned char* data, const size_t size)
			{
				std::lock_guard<std::mutex> Guard(fileHandleMutex);
				return write(fileHandle, data, size);
			}

			bool Receive(std::string& data, const size_t maxData = 1024, const unsigned int timeoutS = 0, const unsigned int timeoutUS = 100000);
			size_t Receive(char* data, const size_t maxData = 1024, const unsigned int timeoutS = 0, const unsigned int timeoutUS = 100000);
			bool ReceiveExact(std::string& data, const size_t length, const unsigned int timeoutS = 0, const unsigned int timeoutUS = 100000);
			size_t ReceiveExact(char* data, const size_t length, const unsigned int timeoutS = 0, const unsigned int timeoutUS = 100000);

		private:
			void Init();
			void Close() { if (fileHandle == -1) return; close(fileHandle); fileHandle = -1; }

			Logger::Logger* logger;
			const std::string tty;
			const unsigned int dataSpeed;
			const unsigned char dataBits;
			const char parity;
			const unsigned char stopBits;
			int fileHandle;
			mutable std::mutex fileHandleMutex;
	};
}
