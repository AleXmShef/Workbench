#include "wbpch.h"
#include "ECSComponent.h"

namespace Workbench {
	uint32_t ECSComponent::s_numComponents = 0;

	ECSComponent::ECSComponent(const UUID* entity) : m_Entity(entity) {
		s_numComponents++;
	}

	ECSComponent::~ECSComponent() {
		s_numComponents--;
	}

	const UUID* ECSComponent::getEntityId() const {
		return m_Entity;
	}

	const UUID* ECSComponent::getUuid() const { 
		return &m_uuid; 
	}
	uint32_t ECSComponent::getNumComponents()
	{
		return s_numComponents;
	}
}
