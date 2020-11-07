#pragma once
#include "wbpch.h"
#include "Utils/UUID.h"

namespace Workbench {

	class ECSEntity {
	public:
		ECSEntity();
		virtual ~ECSEntity();

		const UUID* getUuid();
	protected:
		UUID m_uuid;
	};
}

