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

#include <mutex>
#include <string>
#include <vector>

#include "DataModel/Feedback.h"
#include "DataModel/LayoutItem.h"
#include "DataModel/LockableItem.h"
#include "DataTypes.h"
#include "Logger/Logger.h"

class Manager;

namespace DataModel
{
	class Loco;
	class Street;

	class Track : public LayoutItem, public LockableItem
	{
		public:
			enum SelectStreetApproach : unsigned char
			{
				SelectStreetSystemDefault = 0,
				SelectStreetDoNotCare = 1,
				SelectStreetRandom = 2,
				SelectStreetMinTrackLength = 3,
				SelectStreetLongestUnused = 4
			};

			enum Type : unsigned char
			{
				TrackTypeStraight = 0,
				TrackTypeTurn = 1,
				TrackTypeEnd = 2,
				TrackTypeBridge = 3,
				TrackTypeTunnel = 4,
				TrackTypeTunnelEnd = 5,
				TrackTypeLink = 6
			};

			Track(Manager* manager, const TrackID trackID)
			:	LayoutItem(trackID),
			 	LockableItem(),
			 	manager(manager),
			 	type(TrackTypeStraight),
			 	selectStreetApproach(SelectStreetSystemDefault),
				state(DataModel::Feedback::FeedbackStateFree),
				stateDelayed(DataModel::Feedback::FeedbackStateFree),
			 	locoDirection(DirectionRight),
			 	blocked(false),
			 	locoIdDelayed(LocoNone),
			 	releaseWhenFree(false)
			{
			}

			Track(Manager* manager, const std::string& serialized)
			:	manager(manager)
			{
				Deserialize(serialized);
			}

			ObjectType GetObjectType() const { return ObjectTypeTrack; }

			std::string Serialize() const override;
			bool Deserialize(const std::string& serialized) override;

			bool Reserve(Logger::Logger* logger, const LocoID locoID) override;
			bool ReserveForce(Logger::Logger* logger, const LocoID locoID);
			bool Lock(Logger::Logger* logger, const LocoID locoID) override;
			bool Release(Logger::Logger* logger, const LocoID locoID) override;
			bool ReleaseForce(Logger::Logger* logger, const LocoID locoID);

			Type GetType() const { return type; }
			void SetType(const Type type) { this->type = type; }
			std::vector<FeedbackID> GetFeedbacks() const { return feedbacks; }
			void Feedbacks(const std::vector<FeedbackID>& feedbacks) { this->feedbacks = feedbacks; }

			bool SetFeedbackState(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);
			DataModel::Feedback::FeedbackState GetFeedbackStateDelayed() const { return stateDelayed; };

			bool AddStreet(Street* street);
			bool RemoveStreet(Street* street);

			SelectStreetApproach GetSelectStreetApproach() const { return selectStreetApproach; }
			void SetSelectStreetApproach(const SelectStreetApproach selectStreetApproach) { this->selectStreetApproach = selectStreetApproach; }

			bool GetValidStreets(Logger::Logger* logger, const DataModel::Loco* loco, const bool allowLocoTurn, std::vector<Street*>& validStreets) const;
			Direction GetLocoDirection() const { return locoDirection; }
			void SetLocoDirection(const Direction direction) { locoDirection = direction; }
			bool GetBlocked() const { return blocked; }
			void SetBlocked(const bool blocked) { this->blocked = blocked; }
			LocoID GetLocoDelayed() const { return this->locoIdDelayed; }
			bool GetReleaseWhenFree() const { return releaseWhenFree; }
			void SetReleaseWhenFree(const bool releaseWhenFree) { this->releaseWhenFree = releaseWhenFree; }
			std::string GetLayoutType() const override { return Languages::GetText(Languages::TextTrack); };

		private:
			bool FeedbackStateInternal(const FeedbackID feedbackID, const DataModel::Feedback::FeedbackState state);
			void OrderValidStreets(std::vector<DataModel::Street*>& validStreets) const;
			SelectStreetApproach GetSelectStreetApproachCalculated() const;
			bool ReleaseForceUnlocked(Logger::Logger* logger, const LocoID locoID);

			Manager* manager;
			mutable std::mutex updateMutex;
			Type type;
			std::vector<FeedbackID> feedbacks;
			SelectStreetApproach selectStreetApproach;
			DataModel::Feedback::FeedbackState state;
			DataModel::Feedback::FeedbackState stateDelayed;
			std::vector<Street*> streets;
			Direction locoDirection;
			bool blocked;
			LocoID locoIdDelayed;
			bool releaseWhenFree;
	};
} // namespace DataModel
