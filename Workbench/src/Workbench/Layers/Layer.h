#pragma once
#include "Core.h"
#include "Events/Event.h"
namespace Workbench {
	class WORKBENCH_API Layer {
	public:
		Layer();
		virtual void OnAttach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDetach() = 0;
		virtual ~Layer();

	protected:
		static uint32_t s_numLayers;
	};
}
