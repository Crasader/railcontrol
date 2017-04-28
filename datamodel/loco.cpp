
#include <map>
#include <sstream>

#include "loco.h"

using std::map;
using std::stringstream;
using std::string;

namespace datamodel {

	Loco::Loco(locoID_t locoID, std::string name, controlID_t controlID, protocol_t protocol, address_t address) :
		Object(locoID, name),
		controlID(controlID),
		protocol(protocol),
		address(address),
		speed(0) {
	}

	Loco::Loco(const std::string& serialized) {
		deserialize(serialized);
	}

	std::string Loco::serialize() const {
		stringstream ss;
		ss << "objectType=Loco;" << Object::serialize() << ";controlID=" << (int)controlID << ";protocol=" << (int)protocol << ";address=" << (int)address;
		return ss.str();
	}

	bool Loco::deserialize(const std::string& serialized) {
		map<string,string> arguments;
		parseArguments(serialized, arguments);
		Object::deserialize(arguments);
		if (arguments.count("objectType") && arguments.at("objectType").compare("Loco") == 0) {
			if (arguments.count("controlID")) controlID = stoi(arguments.at("controlID"));
			if (arguments.count("protocol")) protocol = stoi(arguments.at("protocol"));
			if (arguments.count("address")) address = stoi(arguments.at("address"));
			return true;
		}
		return false;
	}

} // namespace datamodel

