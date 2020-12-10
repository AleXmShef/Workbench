#pragma once
#include "wbpch.h"
#include "Core.h"

namespace Workbench{
	struct ForceAccumulator {
		void AddLinearForce(mathfu::vec3 force);
		void AddForceAtPoint(mathfu::vec3 point, mathfu::vec3 force);
		void ClearAccumulators();

		mathfu::vec3 totalLinearForce;	//world space
		mathfu::vec3 totalTorque;		//world space
	};
}

