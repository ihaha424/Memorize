#include "EventBus.h"

#include "IEventListener.h"

#include "InputEventDispatcher.h"

EventBus::EventBus() {
	OBJ_INFO("EventBus is initialized.");

	// Register default event dispatchers
	// Keyboard event dispatcher
	EventTypeID INPUT_EVENT_TYPE_ID = InputEvent::STATIC_EVENT_TYPE_ID;
	EventDispatcher<InputEvent>* keyboardEventDispatcher = new EventDispatcher<InputEvent>();
	_eventDispatcherMap.insert(std::make_pair(INPUT_EVENT_TYPE_ID, keyboardEventDispatcher));

	IEventListener::_eventBus = this;
}

EventBus::~EventBus() {
	// Free the event dispatchers
	for (auto it : _eventDispatcherMap) {
		delete it.second;
		it.second = nullptr;
	}
	_eventDispatcherMap.clear();

	// Clear the event queue
	for (auto& [_, eventQueue] : _eventQueueMap) {
		while (!eventQueue.empty()) {
			delete eventQueue.front();
			eventQueue.pop();
		}
	}
}
