#pragma once

#include <map>
#include <string>
#include <thread>
#include <vector>

#include "manager.h"
#include "network/TcpConnection.h"
#include "webserver/Response.h"

namespace webserver
{
	class WebServer;

	class WebClient
	{
		public:
			WebClient(const unsigned int id, Network::TcpConnection* connection, WebServer &webserver, Manager& m)
			:	logger(Logger::Logger::GetLogger("Webserver")),
				id(id),
				connection(connection),
				run(false),
				server(webserver),
				clientThread(std::thread([this] {Worker();})),
				manager(m),
				buttonID(0)
			{}

			~WebClient();
			void Worker();
			int stop();

		private:
			void interpretClientRequest(const std::vector<std::string>& lines, std::string& method, std::string& uri, std::string& protocol, std::map<std::string,std::string>& arguments, std::map<std::string,std::string>& headers);
			HtmlTag selectLoco();
			HtmlTag selectLayout();
			std::string select(const std::string& name, const std::map<std::string,std::string>& options, const std::string& defaultValue);
			std::string slider(const std::string& name, const std::string& cmd, const unsigned int min, const unsigned int max, const unsigned int value, const std::map<std::string,std::string>& arguments = std::map<std::string,std::string>());
			std::string button(const std::string& value, const std::string& cmd, const std::map<std::string,std::string>& arguments = std::map<std::string,std::string>());
			std::string buttonPopup(const std::string& value, const std::string& cmd, const std::map<std::string,std::string>& arguments = std::map<std::string,std::string>());
			std::string buttonPopupCancel();
			std::string buttonPopupOK();
			void printLoco(const std::map<std::string, std::string>& arguments);
			void printMainHTML();
			void HtmlReplyWithHeader(const HtmlTag& tag);
			void HtmlReplyWithHeaderAndParagraph(const std::string& content) { HtmlReplyWithHeader(HtmlTag("p").AddContent(content)); }
			void HtmlReplyWithHeaderAndParagraph(const char* content) { HtmlReplyWithHeaderAndParagraph(std::string(content)); }
			void deliverFile(const std::string& file);
			void deliverFileInternal(FILE* f, const char* realFile, const std::string& file);
			HtmlTag ControlArgumentTag(const unsigned char argNr, const argumentType_t type, const std::string& value);
			HtmlTag HtmlTagProtocol(const controlID_t controlID, const protocol_t selectedProtocol);
			void handleControlEdit(const std::map<std::string, std::string>& arguments);
			void handleControlSave(const std::map<std::string, std::string>& arguments);
			void handleControlList(const std::map<std::string, std::string>& arguments);
			void handleControlAskDelete(const std::map<std::string, std::string>& arguments);
			void handleControlDelete(const std::map<std::string, std::string>& arguments);
			void handleLocoSpeed(const std::map<std::string,std::string>& arguments);
			void handleLocoDirection(const std::map<std::string,std::string>& arguments);
			void handleLocoFunction(const std::map<std::string, std::string>& arguments);
			void handleLocoEdit(const std::map<std::string, std::string>& arguments);
			void handleLocoSave(const std::map<std::string, std::string>& arguments);
			void handleLocoList(const std::map<std::string, std::string>& arguments);
			void handleLocoAskDelete(const std::map<std::string, std::string>& arguments);
			void handleLocoDelete(const std::map<std::string, std::string>& arguments);
			void handleProtocolLoco(const std::map<std::string, std::string>& arguments);
			void handleLayout(const std::map<std::string,std::string>& arguments);
			void handleAccessoryEdit(const std::map<std::string,std::string>& arguments);
			void handleAccessorySave(const std::map<std::string,std::string>& arguments);
			void handleAccessoryState(const std::map<std::string,std::string>& arguments);
			void handleAccessoryAskDelete(const std::map<std::string,std::string>& arguments);
			void handleAccessoryDelete(const std::map<std::string,std::string>& arguments);
			void handleAccessoryGet(const std::map<std::string,std::string>& arguments);
			void handleSwitchEdit(const std::map<std::string,std::string>& arguments);
			void handleSwitchSave(const std::map<std::string,std::string>& arguments);
			void handleSwitchState(const std::map<std::string,std::string>& arguments);
			void handleSwitchAskDelete(const std::map<std::string,std::string>& arguments);
			void handleSwitchDelete(const std::map<std::string,std::string>& arguments);
			void handleSwitchGet(const std::map<std::string,std::string>& arguments);
			void handleTrackEdit(const std::map<std::string,std::string>& arguments);
			void handleTrackSave(const std::map<std::string,std::string>& arguments);
			void handleTrackAskDelete(const std::map<std::string,std::string>& arguments);
			void handleTrackDelete(const std::map<std::string,std::string>& arguments);
			void handleTrackGet(const std::map<std::string, std::string>& arguments);
			void handleUpdater(const std::map<std::string,std::string>& arguments);
			void UrlDecode(std::string& argumentValue);
			char ConvertHexToInt(char c);
			void WorkerImpl();

			Logger::Logger* logger;
			unsigned int id;
			Network::TcpConnection* connection;
			volatile unsigned char run;
			WebServer& server;
			std::thread clientThread;
			Manager& manager;
			bool headOnly;
			unsigned int buttonID;
	};

}; // namespace webserver

