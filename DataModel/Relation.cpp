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

#include <map>
#include <sstream>
#include <string>

#include "DataModel/Relation.h"
#include "Manager.h"
#include "Utils/Utils.h"

using std::map;
using std::stringstream;
using std::string;

namespace DataModel
{
	std::string Relation::Serialize() const
	{
		stringstream ss;
		ss << LockableItem::Serialize()
			<< ";objectType1=" << static_cast<int>(objectType1)
			<< ";objectID1=" << objectID1
			<< ";objectType2=" << static_cast<int>(objectType2)
			<< ";objectID2=" << objectID2
			<< ";type=" << type
			<< ";priority=" << static_cast<int>(priority)
			<< ";data=" << static_cast<int>(data);
		return ss.str();
	}

	bool Relation::Deserialize(const std::string& serialized)
	{
		map<string,string> arguments;
		ParseArguments(serialized, arguments);
		LockableItem::Deserialize(arguments);
		objectType1 = static_cast<objectType_t>(Utils::Utils::GetIntegerMapEntry(arguments, "objectType1"));
		objectID1 = Utils::Utils::GetIntegerMapEntry(arguments, "objectID1");
		objectType2 = static_cast<objectType_t>(Utils::Utils::GetIntegerMapEntry(arguments, "objectType2"));
		objectID2 = Utils::Utils::GetIntegerMapEntry(arguments, "objectID2");
		type = static_cast<type_t>(Utils::Utils::GetIntegerMapEntry(arguments, "type", TypeCalculate));
		if (type == TypeCalculate)
		{
			switch (objectType1)
			{
				case ObjectTypeStreet:
					type = TypeStreetAtLock;
					break;

				case ObjectTypeLoco:
					type = TypeLocoSlave;
					break;

				default:
					break;
			}
		}
		priority = Utils::Utils::GetIntegerMapEntry(arguments, "priority");
		data = Utils::Utils::GetIntegerMapEntry(arguments, "accessoryState"); // FIXME: remove later
		data = Utils::Utils::GetIntegerMapEntry(arguments, "data", data);
		return true;
	}

	bool Relation::Execute(Logger::Logger* logger, const delay_t delay)
	{
		switch (objectType2)
		{
			case ObjectTypeAccessory:
			{
				bool ret = manager->AccessoryState(ControlTypeInternal, objectID2, static_cast<accessoryState_t>(data), true);
				if (ret == false)
				{
					return false;
				}
				break;
			}

			case ObjectTypeSwitch:
			{
				bool ret = manager->SwitchState(ControlTypeInternal, objectID2, static_cast<switchState_t>(data), true);
				if (ret == false)
				{
					return false;
				}
				break;
			}

			case ObjectTypeSignal:
			{
				bool ret = manager->SignalState(ControlTypeInternal, objectID2, static_cast<signalState_t>(data), true);
				if (ret == false)
				{
					return false;
				}
				break;
			}

			case ObjectTypeTrack:
				manager->TrackSetLocoDirection(objectID2, static_cast<direction_t>(data));
				return true;


			case ObjectTypeStreet:
				return manager->StreetExecute(logger, objectID2);

			case ObjectTypeLoco:
				manager->LocoFunction(ControlTypeInternal, GetLoco(), static_cast<function_t>(objectID2), static_cast<bool>(data));
				return true;

			default:
				return false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		return true;
	}

	LockableItem* Relation::GetObject2()
	{
		switch (objectType2)
		{
			case ObjectTypeAccessory:
				return manager->GetAccessory(objectID2);

			case ObjectTypeSwitch:
				return manager->GetSwitch(objectID2);

			case ObjectTypeTrack:
				return manager->GetTrack(objectID2);

			case ObjectTypeSignal:
				return manager->GetSignal(objectID2);

			case ObjectTypeStreet:
				return manager->GetStreet(objectID2);

			default:
				return nullptr;
		}
	}

	bool Relation::Reserve(Logger::Logger* logger, const locoID_t locoID)
	{
		bool ret = LockableItem::Reserve(logger, locoID);
		if (ret == false)
		{
			logger->Debug(Languages::TextUnableToReserve);
			return false;
		}

		if (ObjectType2() == ObjectTypeLoco)
		{
			return true;
		}

		LockableItem* lockable = GetObject2();
		if (lockable == nullptr)
		{
			logger->Debug(Languages::TextRelationTargetNotFound);
			LockableItem::Release(locoID);
			return false;
		}

		Street* street = dynamic_cast<Street*>(lockable);
		if (street != nullptr)
		{
			return street->Reserve(logger, locoID);
		}

		return lockable->Reserve(logger, locoID);
	}

	bool Relation::Lock(Logger::Logger* logger, const locoID_t locoID)
	{
		bool ret = LockableItem::Lock(logger, locoID);
		if (ret == false)
		{
			return false;
		}

		if (ObjectType2() == ObjectTypeLoco)
		{
			return true;
		}

		LockableItem* lockable = GetObject2();
		if (lockable == nullptr)
		{
			LockableItem::Release(locoID);
			return false;
		}

		Street* street = dynamic_cast<Street*>(lockable);
		if (street != nullptr)
		{
			return street->Lock(logger, locoID);
		}

		bool retLockable = lockable->Lock(logger, locoID);
		if (retLockable == true)
		{
			return true;
		}

		Object* object = dynamic_cast<Object*>(lockable);
		if (object == nullptr)
		{
			return false;
		}

		logger->Debug(Languages::TextUnableToLock, object->GetName());
		return false;
	}

	bool Relation::Release(const locoID_t locoID)
	{
		LockableItem* object = GetObject2();
		if (object != nullptr)
		{
			object->Release(locoID);
		}
		return LockableItem::Release(locoID);
	}
} // namespace DataModel

