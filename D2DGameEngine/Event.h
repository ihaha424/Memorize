#pragma once

#include "IEvent.h"

template<typename E>
class Event : public IEvent {

public:
	static const EventTypeID STATIC_EVENT_TYPE_ID;

	Event() : IEvent(STATIC_EVENT_TYPE_ID) {}
};

template<typename E>
const EventTypeID Event<E>::STATIC_EVENT_TYPE_ID{ typeid(E).hash_code() };
