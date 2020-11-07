#pragma once
#include "Core.h"
#include "Events/Event.h"
namespace Workbench {
	class WORKBENCH_API Layer {
	public:
		Layer();
		virtual void OnAttach() {};
		virtual void OnUpdate() {};
		virtual void OnDetach() {};
		virtual ~Layer();

		static uint32_t GetNumberOfLayers();

	protected:
		static uint32_t s_numLayers;
	};
}
