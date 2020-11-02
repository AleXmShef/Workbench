#include "wbpch.h"
#include "Layer.h"

namespace Workbench {
	Layer::Layer() {
		s_numLayers++;
	}

	Layer::~Layer() {
		s_numLayers--;
	}
}
