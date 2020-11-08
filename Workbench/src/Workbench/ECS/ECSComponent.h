#pragma once
#include "Utils/UUID.h"

namespace Workbench {
	class ECSComponent {
	public:
		ECSComponent();
		virtual ~ECSComponent();

		const UUID* getUuid() const;
	private:
		UUID m_uuid;

	private:
		static uint32_t s_numComponents;
	};
}

