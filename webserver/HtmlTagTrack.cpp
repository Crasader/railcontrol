#include <sstream>

#include "datamodel/Loco.h"
#include "webserver/HtmlTagTrack.h"

using std::string;
using std::to_string;

namespace webserver
{
	HtmlTagTrack::HtmlTagTrack(const Manager& manager, const datamodel::Track* track)
	{
		layoutPosition_t posX;
		layoutPosition_t posY;
		layoutPosition_t posZ;
		layoutItemSize_t w;
		layoutItemSize_t h;
		datamodel::LayoutItem::layoutRotation_t r;
		track->Position(posX, posY, posZ, w, h, r);
		datamodel::Track::type_t type = track->GetType();
		unsigned int layoutPosX = posX * EdgeLength;
		unsigned int layoutPosY = posY * EdgeLength;

		const string& trackName = track->GetName();
		bool occupied = track->GetFeedbackStateDelayed() == datamodel::Feedback::FeedbackStateOccupied;

		locoID_t locoID = track->GetLocoDelayed();
		bool reserved = locoID != LocoNone;

		bool blocked = track->GetBlocked();

		HtmlTag div1("div");
		string trackIdString = to_string(track->GetID());
		string id("t_" + trackIdString);
		div1.AddAttribute("id", id);
		div1.AddClass("layout_item");
		div1.AddClass("track_item");
		string trackClass;
		if (reserved && occupied)
		{
			trackClass = "track_reserved_occupied";
		}
		else if (reserved)
		{
			trackClass = "track_reserved";
		}
		else if (occupied)
		{
			trackClass = "track_occupied";
		}
		else if (blocked)
		{
			trackClass = "track_blocked";
		}
		else
		{
			trackClass = "track_free";
		}

		div1.AddClass(trackClass);
		div1.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");
		std::string image;
		layoutItemSize_t trackHeight = track->GetHeight();
		const string layoutHeight = to_string(EdgeLength * trackHeight);

		switch (type)
		{
			case datamodel::Track::TrackTypeTurn:
				image = "<polygon class=\"track\" points=\"0,22 0,14 22,36 14,36\"/>";
				break;

			case datamodel::Track::TrackTypeEnd:
				image = "<polygon class=\"track\" points=\"14,5 22,5 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<polygon class=\"track\" points=\"4,10 4,5 32,5 32,10\"/>";
				break;

			case datamodel::Track::TrackTypeBridge:
			{
				const string l1 = to_string(EdgeLength * trackHeight - 5);
				const string l2 = to_string(EdgeLength * trackHeight - 3);
				image = "<polygon class=\"track\" points=\"14,0 22,0 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<polygon class=\"track\" points=\"10,3 12,5 12," + l1 + " 10," + l2 + "\"/>";
				image += "<polygon class=\"track\" points=\"26,3 24,5 24," + l1 + " 26," + l2 + "\"/>";
				break;
			}

			case datamodel::Track::TrackTypeTunnel:
				image = "<polygon class=\"track\" points=\"14,0 22,0 22,12 14,12\"/>";
				image += "<polygon class=\"track\" points=\"5,1 10,1 10,10 12,12 24,12 26,10 26,1 31,1 31,3 28,3 28,11 25,14 11,14 8,11 8,3 5,3 \"/>";

			case datamodel::Track::TrackTypeTunnelEnd:
			{
				const string l0 = to_string(EdgeLength * trackHeight - 14);
				const string l1 = to_string(EdgeLength * trackHeight - 12);
				const string l2 = to_string(EdgeLength * trackHeight - 11);
				const string l3 = to_string(EdgeLength * trackHeight - 10);
				const string l4 = to_string(EdgeLength * trackHeight - 8);
				const string l5 = to_string(EdgeLength * trackHeight - 3);
				const string l6 = to_string(EdgeLength * trackHeight - 1);
				image += "<polygon class=\"track\" points=\"14," + l1 + " 22," + l1 + " 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<polygon class=\"track\" points=\"5," + l6 + " 10," + l6 + " 10," + l4 + " 12," + l1 + " 24," + l1 + " 26," + l4 + " 26," + l6 + " 31," + l6 + " 31," + l5 + " 28," + l5 + " 28," + l3 + " 25," + l0 + " 11," + l0 + " 8," + l3 + " 8," + l5 + " 5," + l5 + "\"/>";
				break;
			}

			case datamodel::Track::TrackTypeLink:
				image = "<polygon class=\"track\" points=\"14,22 22,22 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<polygon class=\"track\" points=\"18,1 4,22 32,22\"/>";
				break;

			case datamodel::Track::TrackTypeStraight:
			default:
				const string& directionSign = track->GetLocoDirection() == DirectionRight ? "&rarr; " : "&larr; ";
				const string& locoName = reserved ? directionSign + manager.GetLocoName(locoID) : "";
				const string textPositionX = to_string(EdgeLength * trackHeight - 1);
				image = "<polygon class=\"track\" points=\"14,0 22,0 22," + layoutHeight + " 14," + layoutHeight + "\"/>";
				image += "<text class=\"loconame\" x=\"-" + textPositionX + "\" y=\"11\" id=\"" + id + "_text_loconame\" transform=\"rotate(270 0,0)\" font-size=\"14\">" + locoName + "</text>";
				image += "<text class=\"trackname\" x=\"-" + textPositionX + "\" y=\"33\" id=\"" + id + "_text_trackname\" transform=\"rotate(270 0,0)\" font-size=\"14\">" + trackName + "</text>";
				break;
		}

		int translate = 0;
		if (trackHeight > Height1)
		{
			datamodel::LayoutItem::layoutRotation_t trackRotation = track->GetRotation();
			if (trackRotation == datamodel::LayoutItem::Rotation90 || trackRotation == datamodel::LayoutItem::Rotation270)
			{
				translate = (((trackHeight - 1) * EdgeLength) / 2);
			}
			if (trackRotation == datamodel::LayoutItem::Rotation90)
			{
				translate = -translate;
			}
		}

		div1.AddChildTag(HtmlTag().AddContent("<svg width=\"" + EdgeLengthString + "\" height=\"" + layoutHeight + "\" id=\"" + id + "_img\" style=\"transform:rotate(" + datamodel::LayoutItem::Rotation(track->GetRotation()) + "deg) translate(" + to_string(translate) + "px," + to_string(translate) + "px);\">" + image + "</svg>"));
		div1.AddChildTag(HtmlTag("span").AddClass("tooltip").AddContent(trackName));
		div1.AddAttribute("oncontextmenu", "return onContextLayoutItem(event, '" + id + "');");
		AddChildTag(div1);

		HtmlTag div2("div");
		div2.AddClass("contextmenu");
		div2.AddClass(reserved ? "loco_known" : "loco_unknown");
		div2.AddClass(blocked ? "track_blocked" : "track_unblocked");
		div2.AddAttribute("id", id + "_context");
		div2.AddAttribute("style", "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;");
		div2.AddChildTag(HtmlTag("ul").AddClass("contextentries")
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent(trackName))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddClass("track_block").AddContent("Block track").AddAttribute("onClick", "fireRequestAndForget('/?cmd=trackblock&track=" + trackIdString + "&blocked=true');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddClass("track_unblock").AddContent("Unblock track").AddAttribute("onClick", "fireRequestAndForget('/?cmd=trackblock&track=" + trackIdString + "&blocked=false');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddClass("track_release").AddContent("Release track").AddAttribute("onClick", "fireRequestAndForget('/?cmd=trackrelease&track=" + trackIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddClass("track_loco_release").AddContent("Release loco").AddAttribute("onClick", "fireRequestAndForget('/?cmd=locorelease&track=" + trackIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddClass("track_set").AddContent("Set loco").AddAttribute("onClick", "loadPopup('/?cmd=tracksetloco&track=" + trackIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddClass("track_start_loco").AddContent("Start loco").AddAttribute("onClick", "fireRequestAndForget('/?cmd=trackstartloco&track=" + trackIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddClass("track_stop_loco").AddContent("Stop loco").AddAttribute("onClick", "fireRequestAndForget('/?cmd=trackstoploco&track=" + trackIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent("Edit").AddAttribute("onClick", "loadPopup('/?cmd=trackedit&track=" + trackIdString + "');"))
			.AddChildTag(HtmlTag("li").AddClass("contextentry").AddContent("Delete").AddAttribute("onClick", "loadPopup('/?cmd=trackaskdelete&track=" + trackIdString + "');"))
			);
		AddChildTag(div2);
	}
};
