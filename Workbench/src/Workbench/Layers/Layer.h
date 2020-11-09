#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Time/Timer.h"

namespace Workbench {
	class WORKBENCH_API Layer {
	public:
		Layer();
		virtual void OnAttach() {};
		virtual void OnUpdate(WB_GAME_TIMER* timer) {};
		virtual void OnDetach() {};
		virtual ~Layer();

		static uint32_t GetNumberOfLayers();

	protected:
		static uint32_t s_numLayers;
	};
}
