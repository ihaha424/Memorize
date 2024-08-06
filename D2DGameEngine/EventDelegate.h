#pragma once

#include "IEventDelegate.h"

/**
 * @brief The delegation of an event listner and handler.
 * @tparam T The type of the event listner
 * @tparam EventType The type of the event to be handled
 */
template<typename T, typename EventType>
class EventDelegate : public IEventDelegate {
	using CallbackType = void (T::*)(const EventType* const);
	using EventCallback = std::function<void(T*, const EventType* const)>;

	T* _eventListener;
	EventCallback _eventCallback;

public:
	EventDelegate(T* eventListener, const EventCallback& eventCallback) :
		_eventListener{ eventListener }, _eventCallback{ eventCallback } {}


	void Invoke(const IEvent* const e) override {
		_eventCallback(_eventListener, reinterpret_cast<const EventType* const>(e));
	}

	EventDelegateID GetDelegateTypeID() const override {
		static const EventDelegateID EVENT_DELEGATE_ID{
			typeid(T).hash_code() ^ (typeid(EventType).hash_code() * 0x9e3779b97f4a7c55)
		};

		return EVENT_DELEGATE_ID;
	}

	EventTypeID GetStaticEventTypeID() const override {
		static const EventTypeID STATIC_EVENT_TYPE_ID{
			EventType::STATIC_EVENT_TYPE_ID
		};

		return STATIC_EVENT_TYPE_ID;
	}

	bool Equals(const IEventDelegate* other) const override {
		if (other == nullptr) return false;
		// Shares the same type T and EventType
		if (GetDelegateTypeID() != other->GetDelegateTypeID()) return false;
		// Can be converted to the type of `this`
		EventDelegate<T, EventType>* p = (EventDelegate<T, EventType>*) other;
		// Check if all member variables are the same
		return (_eventListener == p->_eventListener) &&
			(_eventCallback.template target<CallbackType>() ==
				p->_eventCallback.template target<CallbackType>());
	}

	IEventDelegate* Clone() override {
		return new EventDelegate(_eventListener, _eventCallback);
	}

	std::size_t Hash() const override {
		return GetDelegateTypeID() ^
			(std::hash<void*>()(_eventListener) << 1) ^
			(std::hash<void*>()((void*)_eventCallback.template target<CallbackType>()) << 2);
	}
};
