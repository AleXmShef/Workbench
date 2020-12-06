#include "wbpch.h"
#include "RandomGenerator.h"

namespace Workbench {
	std::random_device					RandomGenerator::s_uuid_rd;
	std::mt19937_64						RandomGenerator::s_uuid_gen(s_uuid_rd());
	std::uniform_int_distribution<>		RandomGenerator::s_uuid_unorm_dist1(0, 15);
	std::uniform_int_distribution<>		RandomGenerator::s_uuid_unorm_dist2(8, 11);

	std::string RandomGenerator::generate_uuid_v4() {
		std::stringstream ss;
		int i;
		ss << std::hex;
		for (i = 0; i < 8; i++) {
			ss << s_uuid_unorm_dist1(s_uuid_gen);
		}
		ss << "-";
		for (i = 0; i < 4; i++) {
			ss << s_uuid_unorm_dist1(s_uuid_gen);
		}
		ss << "-";
		for (i = 0; i < 3; i++) {
			ss << s_uuid_unorm_dist1(s_uuid_gen);
		}
		ss << "-";
		ss << s_uuid_unorm_dist2(s_uuid_gen);
		for (i = 0; i < 3; i++) {
			ss << s_uuid_unorm_dist1(s_uuid_gen);
		}
		ss << "-";
		for (i = 0; i < 12; i++) {
			ss << s_uuid_unorm_dist1(s_uuid_gen);
		};
		return ss.str();
	}

	int RandomGenerator::random_in_range(int from, int to) {
		std::uniform_int_distribution<> unorm_dist(from, to);

		return unorm_dist(s_uuid_gen);
	}
}
