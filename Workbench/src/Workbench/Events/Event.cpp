#include "wbpch.h"
#include "Event.h"

namespace Workbench {
	MainEventBus* MainEventBus::m_Instance = new MainEventBus;
}