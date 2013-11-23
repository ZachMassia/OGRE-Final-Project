#pragma once

#include "stdafx.h"

template <typename T>
class Event
{
public:
	// The callback type.
	typedef std::function<void(T)> funcType;

private:
	std::vector<funcType> subscribers;
	const bool isPtr;

public:
	Event() : isPtr(std::is_pointer<T>::value) {}

	// RaiseEvent goes through each subscriber, and passes in `arg`.
	// Any exceptions which occur are silently ignored.
	// 
	// If `arg` is a pointer, it will be deleted when all subscribers
	// have been notified.
	void RaiseEvent(T arg)
	{
		std::for_each(subscribers.cbegin(), subscribers.cend(), [&](const funcType& f) {
			if (f) {
				try { f(arg); }
				catch (...) { }
			}
		});

		// Cleanup the argument if it was a pointer.
		if (isPtr) {
			delete arg;
		}
	}

	// Add a subscriber of type `funcType` to the event.
	//
	// If `sub` is an invalid function, nothing is done.
	void Subscribe(const funcType& sub)
	{
		if (sub) {
			subscribers.push_back(sub);
		}
	}
};