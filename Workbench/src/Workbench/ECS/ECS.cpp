#include "wbpch.h"
#include "ECS.h"

namespace Workbench {
	ECS* ECS::m_instance = nullptr;

	ECS* ECS::GetInstance() {
		if (m_instance == nullptr)
			m_instance = new ECS;
		return m_instance;
	}

	const UUID* ECS::CreateEntity() {
		auto new_entity = new ECSEntity();
		auto new_entity_id = new_entity->getUuid();

		//Check random generator
#ifdef WB_DEBUG
		for (auto entity : m_Entities) {
			if (*(entity->getUuid()) == *new_entity_id)
				WB_CORE_CRITICAL("TRIYING TO CREATE ENTITY WITH THE SAME UUID, RANDOM GENERATION COMPROMISED");
		}
#endif

		m_Entities.emplace_back(new_entity);

		POST_EVENT(new EntityCreatedEvent(new_entity_id));

		return new_entity_id;
	}

	void ECS::DestroyEntity(const UUID* entityId) {
		auto it = m_Entities.begin();
		auto end = m_Entities.end();
		for (; it != end; ++it) {
			if ((*it)->getUuid() == entityId) {

				auto size = m_EntityToComponentsMap[entityId].size();

				for (int i = 0; i < size; i++) {
					auto [comp, id] = *m_EntityToComponentsMap[entityId].rbegin();
					DestroyComponent(entityId, comp, id);
				}

				m_EntityToComponentsMap.erase(entityId);

				m_Entities.erase(it);
				//POST_EVENT(new EntityDestroyedEvent(entityId));
				return;
			}
		}
	}


}
