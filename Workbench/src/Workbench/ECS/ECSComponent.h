#pragma once
#include "Core.h"
#include "Utils/UUID.h"

namespace Workbench {
	class WORKBENCH_API ECSComponent {
	public:
		ECSComponent(const UUID* entity);
		virtual ~ECSComponent();
		const UUID* getEntityId() const;
		const UUID* getUuid() const;

		static uint32_t getNumComponents();
	protected:
		UUID m_uuid;
		const UUID* m_Entity;

	protected:
		static uint32_t s_numComponents;
	};
}

