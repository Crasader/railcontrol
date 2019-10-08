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

#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <thread>

#include "HardwareInterface.h"
#include "HardwareParams.h"
#include "Logger/Logger.h"

// CAN protocol specification at http://streaming.maerklin.de/public-media/cs2/cs2CAN-Protokoll-2_0.pdf

namespace Hardware
{
	class CS2 : HardwareInterface
	{
		public:
			CS2(const HardwareParams* params);
			~CS2();

			bool CanHandleLocos() const override { return true; }
			bool CanHandleAccessories() const override { return true; }
			bool CanHandleFeedback() const override { return true; }

			void GetLocoProtocols(std::vector<protocol_t>& protocols) const override
			{
				protocols.push_back(ProtocolMM2);
				protocols.push_back(ProtocolMFX);
				protocols.push_back(ProtocolDCC);
			}

			bool LocoProtocolSupported(protocol_t protocol) const override
			{
				return (protocol == ProtocolMM2 || protocol == ProtocolMFX || protocol == ProtocolDCC);
			}

			void GetAccessoryProtocols(std::vector<protocol_t>& protocols) const override
			{
				protocols.push_back(ProtocolMM2);
				protocols.push_back(ProtocolDCC);
			}

			bool AccessoryProtocolSupported(protocol_t protocol) const override
			{
				return (protocol == ProtocolMM2 || protocol == ProtocolDCC);
			}

			void GetArgumentTypes(std::map<unsigned char,argumentType_t>& argumentTypes) const override
			{
				argumentTypes[1] = IpAddress;
			}

			void Booster(const boosterState_t status) override;
			void LocoSpeed(const protocol_t& protocol, const address_t& address, const locoSpeed_t& speed) override;
			void LocoDirection(const protocol_t& protocol, const address_t& address, const direction_t& direction) override;
			void LocoFunction(const protocol_t protocol, const address_t address, const function_t function, const bool on) override;
			void Accessory(const protocol_t protocol, const address_t address, const accessoryState_t state, const bool on) override;

		private:
			Logger::Logger* logger;
			volatile unsigned char run;
			struct sockaddr_in sockaddr_inSender;
			int senderSocket;
			std::thread receiverThread;
			static const unsigned short hash = 0x7337;

			typedef unsigned char cs2Prio_t;
			typedef unsigned char cs2Command_t;
			typedef unsigned char cs2Response_t;
			typedef unsigned char cs2Length_t;
			typedef uint32_t cs2Address_t;

			void intToData(const uint32_t i, char* buffer);
			uint32_t dataToInt(const char* buffer);
			uint16_t dataToShort(const char* buffer);
			void createCommandHeader(char* buffer, const cs2Prio_t prio, const cs2Command_t command, const cs2Response_t response, const cs2Length_t length);
			void readCommandHeader(char* buffer, cs2Prio_t& prio, cs2Command_t& command, cs2Response_t& response, cs2Length_t& length, cs2Address_t& address, protocol_t& protocol);
			void createLocID(char* buffer, const protocol_t& protocol, const address_t& address);
			void createAccessoryID(char* buffer, const protocol_t& protocol, const address_t& address);
			void receiver();
			int CreateUdpConnection(const struct sockaddr* sockaddr, const unsigned int sockaddr_len, const char* server, const unsigned short port);
	};

	extern "C" CS2* create_cs2(const HardwareParams* params);
	extern "C" void destroy_cs2(CS2* cs2);

} // namespace

