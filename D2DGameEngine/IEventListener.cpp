#include "IEventListener.h"

EventBus* IEventListener::_eventBus{ nullptr };


IEventListener::IEventListener() {}

IEventListener::~IEventListener() {
	RemoveAllEventHandlers();
}

void IEventListener::RemoveAllEventHandlers() {
	for (IEventDelegate* eventDelegate : _eventDelegateRegistry) {
		_eventBus->RemoveEventDelegate(eventDelegate);
	}
}
