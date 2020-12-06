#pragma once
#include "wbpch.h"
#include "Core.h"

namespace Workbench {
	class WORKBENCH_API RandomGenerator {
	public:
		static std::string generate_uuid_v4();

		static int random_in_range(int from, int to);

	protected:
		static std::random_device				s_uuid_rd;
		static std::mt19937_64					s_uuid_gen;
		static std::uniform_int_distribution<>	s_uuid_unorm_dist1;
		static std::uniform_int_distribution<>	s_uuid_unorm_dist2;
	};
}

