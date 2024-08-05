#pragma once

#include "framework.h"

#include "Debug.h"

#include "EventId.h"

/**
 * @brief The wrapper class of an event listener and handler.
 * It eases of dispatching events to listeners.
 */
class IEvent;
class IEventDelegate {

public:
	virtual void Invoke(const IEvent* const e) = 0;
	virtual EventDelegateID GetDelegateTypeID() const = 0;
	virtual EventTypeID GetStaticEventTypeID() const = 0;
	virtual bool Equals(const IEventDelegate* other) const = 0;
	virtual IEventDelegate* Clone() = 0;
	virtual std::size_t Hash() const = 0;
};

template<>
struct std::equal_to<IEventDelegate*> {
	constexpr
		bool operator()(const IEventDelegate* lhs, const IEventDelegate* rhs) const {
		return lhs->Equals(rhs);
	}
};

template<>
struct std::hash<IEventDelegate*> {
	std::size_t operator()(const IEventDelegate* eventDelegate) const {
		return eventDelegate->Hash();
	}
};
