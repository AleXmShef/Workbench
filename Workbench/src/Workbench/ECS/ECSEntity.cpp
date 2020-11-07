#include "wbpch.h"
#include "ECSEntity.h"

namespace Workbench {
	ECSEntity::ECSEntity() {
		
	}

	ECSEntity::~ECSEntity() {
		
	}

	const UUID* ECSEntity::getUuid() {
		return &m_uuid;
	}
}
