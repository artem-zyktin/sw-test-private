#include "Event.hpp"

namespace sw::core
{
	HandleId EventHandle::GetHandleId() const noexcept
	{
		return _id;
	}

	void EventHandle::Unsubscribe() noexcept
	{
		if (_event)
		{
			_event->Unsubscribe(_id);
		}
	}

	EventHandle::~EventHandle() noexcept
	{
		Unsubscribe();
	}

	EventHandle::EventHandle(IEvent* event, HandleId id) noexcept
		: _event(event)
		, _id(id)
	{
	}
}
