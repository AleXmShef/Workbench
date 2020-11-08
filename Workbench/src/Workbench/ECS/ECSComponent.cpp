#include "wbpch.h"
#include "ECSComponent.h"

namespace Workbench {
	uint32_t ECSComponent::s_numComponents = 0;

	ECSComponent::ECSComponent() {
		s_numComponents++;
	}

	ECSComponent::~ECSComponent() {
		s_numComponents--;
	}

	const UUID* ECSComponent::getUuid() const { 
		return &m_uuid; 
	}
}
