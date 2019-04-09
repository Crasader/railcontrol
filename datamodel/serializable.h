#pragma once

#include <map>
#include <string>

namespace datamodel {

	class Serializable {
		public:
			virtual ~Serializable() {};
			virtual std::string Serialize() const = 0;
			virtual bool Deserialize(const std::string& serialized) = 0;

		protected:
			static void ParseArguments(std::string serialized, std::map<std::string,std::string>& arguments);
	};

} // namespace datamodel

