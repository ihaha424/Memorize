#pragma once

#include "framework.h"

#include "Debug.h"

#include "EventDelegate.h"

#include "EventBus.h"

/**
 * @brief An interface class to listen out for the event happenings.
 */
class IEventListener {
	LOG_REGISTER_OBJ(IEventListener)

		friend class EventBus;
	static EventBus* _eventBus;

	using EventDelegateRegistry = std::unordered_set<IEventDelegate*>;
	EventDelegateRegistry _eventDelegateRegistry;

protected:
	template<class EventType, class T>
	using CallbackType = void(T::*)(const EventType* const);

public:
	IEventListener();
	virtual ~IEventListener();

	template<class EventType, class T>
	EventDelegate<T, EventType>* AddEventHandler(CallbackType<EventType, T> callback) {
		EventTypeID STATIC_EVENT_TYPE_ID{ EventType::STATIC_EVENT_TYPE_ID };
		auto* eventDelegate = new EventDelegate<T, EventType>(static_cast<T*>(this), callback);
		_eventDelegateRegistry.insert(eventDelegate);

		// Add it to the event handler
		if (!_eventBus) {
			OBJ_ERROR(-1, "Event bus is not found!");
			return nullptr;
		}
		_eventBus->AddEventDelegate<EventType>(eventDelegate);

		return eventDelegate;
	}

	template<class EventType, class T>
	void RemoveEventHandler(CallbackType<EventType, T> callback) {
		EventDelegate<T, EventType> eventDelegate(static_cast<T*>(this), callback);
		auto it = _eventDelegateRegistry.find(&eventDelegate);
		if (it == _eventDelegateRegistry.end()) {
			OBJ_WARNING("Deletion of unregistered event listener detected!");
			return;
		}
		_eventDelegateRegistry.erase(it);

		// Remove it from the event handler
		if (!_eventBus) {
			OBJ_ERROR(-1, "Event handler is not found!");
			return;
		}
		_eventBus->RemoveEventDelegate(&eventDelegate);
	}

	void RemoveAllEventHandlers();
};
