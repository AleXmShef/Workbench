#include "wbpch.h"
#include "ECS.h"

namespace Workbench {
	const UUID* ECS::CreateEntity() {
		auto new_entity = new ECSEntity();
		auto new_entity_id = new_entity->getUuid();

		m_Entities.emplace_back(new_entity);

		POST_EVENT(new EntityCreatedEvent(new_entity_id));

		return new_entity_id;
	}

	void ECS::DestroyEntity(const UUID* entityId) {
		auto it = m_Entities.begin();
		auto end = m_Entities.end();
		for (; it != end; ++it) {
			if (*((*it)->getUuid()) == *entityId) {
				//for (auto it = m_Components.begin(); it != m_Components.end(); it++)
					//m_Components[(*it).first].erase(entityId);
				m_Entities.erase(it);
				POST_EVENT(new EntityDestroyedEvent(entityId));
				break;
			}
		}
	}


}
