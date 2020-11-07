#pragma once
#include "Utils/UUID.h"

namespace Workbench {
	class ECSComponent {
	public:
		ECSComponent();
		virtual ~ECSComponent();

		const UUID* getUuid();
	protected:
		UUID m_uuid;

	protected:
		static uint32_t s_numComponents;
	};
}

