#pragma once

#include <string>

namespace Network
{
	class TcpConnection
	{
		public:
			TcpConnection(int socket);
			~TcpConnection();

			void Terminate();
			int Send(const char* buf, const size_t buflen, const int flags);
			int Receive(char* buf, const size_t buflen, const int flags);

		private:
			int connectionSocket;
			volatile bool connected;
	};
}
