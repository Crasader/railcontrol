#pragma once

#include <string>

#include "Logger/LoggerServer.h"
#include "network/TcpServer.h"

namespace Logger
{
	class Logger
	{
		public:
			Logger(LoggerServer& server, const std::string& component)
			:	server(server),
			 	component(component)
			{}

			~Logger() {};

			static Logger* GetLogger(const std::string& component) { return LoggerServer::Instance().GetLogger(component); }

			bool IsComponent(const std::string& component) { return component.compare(this->component) == 0; }

			static std::string Format(const std::string& input) { return input; }
			static std::string Format(char* input) { const char* constInput = input; return Format(constInput); }
			static std::string Format(const char* input)
			{
				if (input == nullptr)
				{
					return std::string("");
				}
				return std::string(input);
			}

			template<typename... Args>
			static std::string Format(const std::string& input, Args... args)
			{
				std::string output = input;
				FormatInternal(output, 0, args...);
				return output;
			}

			template<typename... Args>
			static std::string Format(char* input, Args... args)
			{
				const char* constInput = input;
				return Format(constInput, args...);
			}

			template<typename... Args>
			static std::string Format(const char* input, Args... args)
			{
				if (input == nullptr)
				{
					return std::string("");
				}
				return Format(std::string(input), args...);
			}

			template<typename... Args> void Error(Args... args)
			{
				Log("Error", args...);
			}

			template<typename... Args> void Warning(Args... args)
			{
				Log("Warning", args...);
			}

			template<typename... Args> void Info(Args... args)
			{
				Log(std::string("Info"), args...);
			}

			template<typename... Args> void Debug(Args... args)
			{
				Log("Debug", args...);
			}

			LoggerServer& server;
			const std::string component;

		private:
			static std::string DateTime();

			static void Replace(std::string& workString, const unsigned char argument, const std::string& value);
			static void Replace(std::string& workString, const unsigned char argument, char* value)
			{
				const char* constValue = value;
				Replace(workString, argument, std::string(constValue));
			}
			static void Replace(std::string& workString, const unsigned char argument, const char* value)
			{
				Replace(workString, argument, std::string(value == nullptr ? "" : value));
			}
			template<typename T>
			static void Replace(std::string& workString, const unsigned char argument, T value)
			{
				Replace(workString, argument, std::to_string(value));
			}

			template<typename T>
			static void FormatInternal(std::string& workString,
				const unsigned char argument,
				T value)
			{
				Replace(workString, argument, value);
			}

			template<typename T, typename... Args>
			static void FormatInternal(std::string& workString,
				const unsigned char argument,
				T value,
				Args... args)
			{
				Replace(workString, argument, value);
				FormatInternal(workString, argument + 1, args...);
			}

			template<typename... Args> void Log(const std::string& type, Args... args)
			{
				server.Send(DateTime() + ": " + type + ": " + component + ": " + Format(args...) + "\n");
			}
	};
}