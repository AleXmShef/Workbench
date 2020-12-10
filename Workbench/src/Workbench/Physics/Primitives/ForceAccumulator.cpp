#include "wbpch.h"
#include "ForceAccumulator.h"

namespace Workbench {

	void ForceAccumulator::AddLinearForce(mathfu::vec3 force) {
		totalLinearForce += force;
	}	 
		 
	void ForceAccumulator::AddForceAtPoint(mathfu::vec3 point, mathfu::vec3 force) {
		totalLinearForce += force;
		totalTorque += mathfu::vec3::DotProduct(point, force);
	}

	void ForceAccumulator::ClearAccumulators() {
		totalLinearForce *= 0;
		totalTorque *= 0;
	}
}
