#include "wbpch.h"
#include "Layer.h"

namespace Workbench {
	uint32_t Layer::s_numLayers = 0;

	Layer::Layer() {
		s_numLayers++;
	}

	Layer::~Layer() {
		s_numLayers--;
	}
	uint32_t Layer::GetNumberOfLayers()
	{
		return s_numLayers;
	}
}
