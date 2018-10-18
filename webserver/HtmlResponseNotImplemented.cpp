#include "webserver/HtmlResponseNotImplemented.h"

namespace webserver
{
	using std::string;

	HtmlResponseNotImplemented::HtmlResponseNotImplemented(const string& method)
	: HtmlResponse(HtmlResponse::NotImplemented)
	{
		content.AddChildTag(HtmlTag("h1").AddContent("Method not implemented"));
		content.AddChildTag(HtmlTag("p").AddContent("Method ").AddContent(method).AddContent(" not implemented"));
	}
};
