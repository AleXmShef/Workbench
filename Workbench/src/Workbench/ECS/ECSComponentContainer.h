#pragma once
#include "wbpch.h"
#include "Core.h"
#include "ECSComponent.h"

namespace Workbench {
	class WORKBENCH_API ECSComponentContainer {
	public:
		const UUID* AddComponent(ECSComponent* component);
		ECSComponent* RemoveComponent(const UUID* component_id);
		ECSComponent* RemoveComponent(ECSComponent* component);
		ECSComponent* GetComponentById(const UUID* component_id);
		std::pair<std::vector<ECSComponent*>::iterator, std::vector<ECSComponent*>::iterator> GetComponents();
		std::vector<ECSComponent*>* _GetComponents();

	protected:
		std::vector<ECSComponent*> m_Components;
	};
}

