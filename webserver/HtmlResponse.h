#pragma once

#include <ostream>
#include <string>

#include "Response.h"

namespace webserver
{
	class HtmlResponse : public Response
	{
		public:
			HtmlResponse(const responseCode_t responseCode) : Response(responseCode, HtmlTag("body")) {}
			HtmlResponse(const responseCode_t responseCode, const HtmlTag tag) : Response(responseCode, tag) {}
			~HtmlResponse() {};
			void AddAttribute(const std::string name, const std::string value);
			void AddChildTag(HtmlTag content);

			friend std::ostream& operator<<(std::ostream& stream, const HtmlResponse& response);
	};
}; // namespace webserver
