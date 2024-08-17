#pragma once

#include "framework.h"

#include "Debug.h"

#include "IEventDispatcher.h"

#include "EventDelegate.h"

/**
 * @brief Dispatches a specific type of event to listeners.
 * @tparam EventType The type of the event
 */
template<typename EventType>
class EventDispatcher : public IEventDispatcher {
	LOG_REGISTER_OBJ(EventDispatcher<EventType>)

		using EventDelegateRegistry = std::unordered_set<IEventDelegate*>;
	using DelegateGarbageCollector = std::unordered_set<IEventDelegate*>;

	EventDelegateRegistry _eventDelegateRegistry;
	DelegateGarbageCollector _pendingDelegateRemoves;

	bool _locked;

public:
	EventDispatcher() : _locked{ false } {}
	virtual ~EventDispatcher() {
		_pendingDelegateRemoves.clear();
		_eventDelegateRegistry.clear();
	}

	// Dispatch an event to listeners
	void Dispatch(IEvent* e) override {
		if (!e) return;

		_locked = true;

		// Remove pending delegates
		if (!_pendingDelegateRemoves.empty()) {
			for (auto it = _pendingDelegateRemoves.begin();
				it != _pendingDelegateRemoves.end();) {
				_eventDelegateRegistry.erase(*it);
				it = _pendingDelegateRemoves.erase(it);
			}
		}

		// Invoke callbacks
		for (auto* eventDelegate : _eventDelegateRegistry) {
			eventDelegate->Invoke(e);
		}

		_locked = false;
	}

	virtual void AddEventDelegate(IEventDelegate* eventDelegate) override {
		// Check if the delegate has the same event type.
		if (GetStaticEventTypeID() != eventDelegate->GetStaticEventTypeID()) {
			OBJ_ERROR(-1, dbg::text("Event Type doesn't match.",
				" Dispatcher EventType ID=", GetStaticEventTypeID(),
				" Delegate EventType ID=", eventDelegate->GetStaticEventTypeID()
			));
			return;
		}

		// Check if this event delegate is in the garbage collector
		auto it = _pendingDelegateRemoves.find(eventDelegate);
		// If yes, pull it out from the garbage collector and return
		if (it != _pendingDelegateRemoves.end()) {
			_pendingDelegateRemoves.erase(it);
			return;
		}

		// Otherwise, just add it into the delegate list
		_eventDelegateRegistry.insert(eventDelegate);
	}

	virtual void RemoveEventDelegate(IEventDelegate* eventDelegate) override {
		if (_locked) 
		{
			// Check if the event delegate exists in the list.
			auto it = _eventDelegateRegistry.find(eventDelegate);
			if (it == _eventDelegateRegistry.end()) {
				OBJ_WARNING("Removal of event delegate failed!");
				return;
			}

			// Put into the garbage collector
			_pendingDelegateRemoves.insert(eventDelegate);
		}
		else 
		{
			// Check if the event delegate exists in the list.
			auto it = _eventDelegateRegistry.find(eventDelegate);
			if (it == _eventDelegateRegistry.end()) {
				OBJ_WARNING("Removal of event delegate failed!");
				return;
			}

			// Delete it immediately
			delete* it;
			_eventDelegateRegistry.erase(it);
		}
	}

	virtual std::size_t GetEventDelegateCount() const override {
		return _eventDelegateRegistry.size();
	}

	EventTypeID GetStaticEventTypeID() const override {
		static const EventTypeID STATIC_EVENT_TYPE_ID{
			EventType::STATIC_EVENT_TYPE_ID
		};

		return STATIC_EVENT_TYPE_ID;
	}
};
