#pragma once
#pragma warning(disable : 4251)
#include "wbpch.h"
#include "Core.h"
#include "Random/RandomGenerator.h"

namespace Workbench {
	struct WORKBENCH_API UUID {
	public:
		UUID() : string_uuid(RandomGenerator::generate_uuid_v4()), hashed_uuid((int32_t)std::hash<std::string>{}(string_uuid)) {};

		const std::string& str() const { return string_uuid; }
		int32_t uuid() { return hashed_uuid; }

		bool operator==(const UUID& rhs) const {
			return hashed_uuid == rhs.hashed_uuid;
		}

	protected:
		const std::string string_uuid;
		const uint32_t hashed_uuid;
	};
}