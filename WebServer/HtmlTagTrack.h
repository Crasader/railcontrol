/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2020 Dominik (Teddy) Mahrer - www.railcontrol.org

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

#include <string>

#include "WebServer/HtmlTagTrackBase.h"

class Manager;

namespace DataModel
{
	class LayoutItem;
	class Track;
	class TrackBase;
}

namespace WebServer
{
	class HtmlTagTrack : public HtmlTagTrackBase
	{
		public:
			HtmlTagTrack() = delete;
			HtmlTagTrack(const Manager& manager, const DataModel::Track* track)
			:	HtmlTagTrackBase(manager, track->GetTrackType(), dynamic_cast<const DataModel::TrackBase*>(track), dynamic_cast<const DataModel::LayoutItem*>(track))
			{}

			virtual ~HtmlTagTrack() {}
	};
}; // namespace WebServer

