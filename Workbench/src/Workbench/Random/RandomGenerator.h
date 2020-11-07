#pragma once
#include "wbpch.h"

namespace Workbench {
	class RandomGenerator {
	public:
		static std::string generate_uuid_v4();
	protected:
		static std::random_device	s_uuid_rd;
		static std::mt19937_64		s_uuid_gen;
		static std::uniform_int_distribution<> s_uuid_unorm_dist1;
		static std::uniform_int_distribution<> s_uuid_unorm_dist2;
	};
}

