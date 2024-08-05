#pragma once

#include "framework.h"

#include "Singleton.h"

#include "Debug.h"

#include "IEvent.h"
#include "EventDispatcher.h"

class IEventListener;
class EventBus : public Singleton<EventBus> {
	LOG_REGISTER_OBJ(EventBus)

	friend class IEventListener;

	using Super = Singleton<EventBus>;
	using EventDispatcherMap = std::unordered_map<EventTypeID, IEventDispatcher*>;
	using EventQueueMap = std::unordered_map<EventTypeID, std::queue<IEvent*>>;

	/*
	* - EventDispatcher is a collection of EventDelegate
	*		for a certain type of event.
	*
	*	- EventDispatcherMap
	*	+------------+-----------------------------------------------------------+
	*	| EventType0 | EventDelegate0_0, EventDelegate0_1, ..., EventDelegate0_M |
	*	+------------+-----------------------------------------------------------+
	*	| EventType1 | EventDelegate1_0, EventDelegate1_1, ..., EventDelegate1_M |
	*	+------------+-----------------------------------------------------------+
	*	|    ...     |                               ...                         |
	*	+------------+-----------------------------------------------------------+
	*	| EventTypeN | EventDelegateN_0, EventDelegateN_1, ..., EventDelegateN_M |
	*	+------------+-----------------------------------------------------------+
	*
	*	- EventQueueMap
	*	+------------+-----------------------------------------------+
	*	| EventType0 | -> Event0_0 -> Event0_1 -> ... -> Event0_M -> |
	*	+------------+-----------------------------------------------+
	*	| EventType1 | -> Event1_0 -> Event1_1 -> ... -> Event1_M -> |
	*	+------------+-----------------------------------------------+
	*	|    ...     |                    ...                        |
	*	+------------+-----------------------------------------------+
	*	| EventTypeN | -> EventN_0 -> EventN_1 -> ... -> EventN_M -> |
	*	+------------+-----------------------------------------------+
	*
	*/

	EventDispatcherMap _eventDispatcherMap;
	EventQueueMap _eventQueueMap;

public:
	EventBus();
	~EventBus();

	void ClearEventRegistry() {
		_eventQueueMap.clear();
	}

	void ClearEventDispatcher() {
		_eventDispatcherMap.clear();
	}

	template<class EventType, typename... Args>
		requires requires (Args&&... eventArgs) {
		EventType(std::forward<Args>(eventArgs)...);
	}
	void PushEvent(Args&&... eventArgs) {
		EventTypeID EVENT_TYPE_ID = EventType::STATIC_EVENT_TYPE_ID;
		OBJ_VERBOSE(dbg::text("Received an event - ", typeid(EventType).name()));
		EventType* e = new EventType(std::forward<Args>(eventArgs)...);
		_eventQueueMap[EVENT_TYPE_ID].push(e);
	}

	void DispatchAllEvents() {
		// Send out and dispatch all events
		for (auto& [eventID, eventQueue] : _eventQueueMap) {
			while (!eventQueue.empty()) {
				IEvent* e = eventQueue.front();
				_eventDispatcherMap[eventID]->Dispatch(e);
				eventQueue.pop();
			}
		}
	}

	template<typename... Events>
	void DispatchEvent() {
		std::initializer_list<EventTypeID> ilist = { Events::STATIC_EVENT_TYPE_ID... };

		for (EventTypeID EVENT_TYPE_ID : ilist) {
			auto it = _eventQueueMap.find(EVENT_TYPE_ID);
			if (it == _eventQueueMap.end()) continue;

			auto& [_, eventQueue] = *it;
			while (!eventQueue.empty()) {
				IEvent* e = eventQueue.front();
				_eventDispatcherMap[EVENT_TYPE_ID]->Dispatch(e);
				eventQueue.pop();
			}
		}

	}

private:
	template<class EventType>
	void AddEventDelegate(IEventDelegate* eventDelegate) {
		EventTypeID EVENT_TYPE_ID = EventType::STATIC_EVENT_TYPE_ID;

		// If the event dispatcher for a specific event
		// doesn't exist in the map, create one.
		auto it = _eventDispatcherMap.find(EVENT_TYPE_ID);
		if (it == _eventDispatcherMap.end()) {
			EventDispatcher<EventType>* dispatcher = new EventDispatcher<EventType>();
			dispatcher->AddEventDelegate(eventDelegate);
			_eventDispatcherMap.insert(std::make_pair(EVENT_TYPE_ID, dispatcher));
			return;
		}

		// If exists, add an event delegate to the map.
		_eventDispatcherMap[EVENT_TYPE_ID]->AddEventDelegate(eventDelegate);
	}

	void RemoveEventDelegate(IEventDelegate* eventDelegate) {
		EventTypeID EVENT_TYPE_ID = eventDelegate->GetStaticEventTypeID();
		auto it = _eventDispatcherMap.find(EVENT_TYPE_ID);
		if (it != _eventDispatcherMap.end()) {
			_eventDispatcherMap[EVENT_TYPE_ID]->RemoveEventDelegate(eventDelegate);
		}
	}
};
