#pragma once
#include "Utils/UUID.h"

namespace Workbench {
	class ECSComponent {
	public:
		ECSComponent(const UUID* entity);
		virtual ~ECSComponent();
		const UUID* getEntityId() const;
		const UUID* getUuid() const;
	protected:
		UUID m_uuid;
		const UUID* m_Entity;

	protected:
		static uint32_t s_numComponents;
	};
}

