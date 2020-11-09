#include "wbpch.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "ECS/ECS.h"

namespace Workbench {
	void PhysicsSystem::OnUpdate(WB_GAME_TIMER* timer) {
		auto [it, end] = ECS::getInstance()->GetComponents<PhysicsComponent>();
		for (; it != end; ++it) {
			auto component = *it;
			auto [x, y, z, vel, acc] = component->data;

			//WB_CORE_TRACE("Updateing physics component:");
			//WB_CORE_TRACE("\tx: {0}, y: {1}, z: {2}", x, y, z);
			//WB_CORE_TRACE("\tvel: {}", vel);
			//WB_CORE_TRACE("\tacc: {}", acc);

			component->data = { x, y + vel * timer->GetTickTime(), z, vel + acc * timer->GetTickTime(), acc };
		}
	}
}
