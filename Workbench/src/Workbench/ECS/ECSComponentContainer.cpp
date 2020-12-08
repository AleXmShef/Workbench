#include "wbpch.h"
#include "ECSComponentContainer.h"

namespace Workbench {

	const UUID* ECSComponentContainer::AddComponent(ECSComponent* component) {
		for (auto ex_component : m_Components) {
			if (ex_component->getUuid() == component->getUuid()) {
				return ex_component->getUuid();
			}
		}
		m_Components.push_back(component);
		return component->getUuid();
	}

	ECSComponent* ECSComponentContainer::RemoveComponent(const UUID* component_id) {
		auto it = m_Components.begin();
		auto end = m_Components.end();
		for (; it != end; ++it) {
			auto component = *it;
			if (component->getUuid() == component_id) {
				m_Components.erase(it);
				return component;
			}
		}
		return nullptr;
	}

	ECSComponent* ECSComponentContainer::RemoveComponent(ECSComponent* component) {
		return RemoveComponent(component->getUuid());
	}

	ECSComponent* ECSComponentContainer::GetComponentById(const UUID* component_id) {
		for (auto ex_component : m_Components) {
			if (ex_component->getUuid() == component_id) {
				return ex_component;
			}
		}
		return nullptr;
	}

	std::pair<std::vector<ECSComponent*>::iterator, std::vector<ECSComponent*>::iterator> ECSComponentContainer::GetComponents() {
		return { m_Components.begin(), m_Components.end() };
	}

	std::vector<ECSComponent*>* ECSComponentContainer::_GetComponents() {
		return &m_Components;
	};

}
