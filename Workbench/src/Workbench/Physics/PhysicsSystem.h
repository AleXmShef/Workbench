#pragma once
#include "wbpch.h"
#include "Layers/Layer.h"

namespace Workbench {
	class PhysicsSystem : public Layer {
	public:
		void OnUpdate(WB_GAME_TIMER* timer) override;
	protected:

	};
}
