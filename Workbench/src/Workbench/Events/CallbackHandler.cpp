#include "wbpch.h"
#include "CallbackHandler.h"

namespace Workbench {
	AbstractCallbackHandler::~AbstractCallbackHandler() {};

	void AbstractCallbackHandler::exec(const AbstractEvent* event) {
		call(event);
	}
}
