#pragma once

#include "framework.h"

#include "Event_Types.h"

class IEvent {
	EventTypeID _eventTypeID;

public:
	IEvent(EventTypeID eventTypeID) : _eventTypeID{ eventTypeID } {}

	const EventTypeID GetEventTypeID() const { return _eventTypeID; }
};
