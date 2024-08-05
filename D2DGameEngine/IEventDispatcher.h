#pragma once

#include "framework.h"

#include "EventId.h"

class IEvent;
class IEventDelegate;
class IEventDispatcher {

public:
	virtual ~IEventDispatcher() {}
	virtual void Dispatch(IEvent* event) = 0;
	virtual void AddEventDelegate(IEventDelegate* eventDelegate) = 0;
	virtual void RemoveEventDelegate(IEventDelegate* eventDelegate) = 0;
	virtual std::size_t GetEventDelegateCount() const = 0;
	virtual EventTypeID GetStaticEventTypeID() const = 0;
};
