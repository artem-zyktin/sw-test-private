#pragma once
#pragma once

#include "Event.hpp"

namespace sw::core
{
	template<typename ...Ts>
	Event<Ts...>::Event()
		: _slots()
		, _slotMap()
	{
		_slots.reserve(8);
	}

	template<typename ...Ts>
	Event<Ts...>::~Event()
	{
		for (std::unique_ptr<Slot>& item : _slots)
		{
			item->_eventHandle->_event = nullptr;
		}
	}

	template<typename ...Ts>
	void Event<Ts...>::Invoke(Ts... args)
	{
		for (std::unique_ptr<Slot>& item : _slots)
		{
			item->Invoke(args...);
		}
	}

	template<typename ...Ts>
	template<typename Fn>
	inline std::shared_ptr<EventHandle> sw::core::Event<Ts...>::Subscribe(Fn&& fn)
	{
		HandleId id = _slots.size();
		auto handle = std::make_shared<EventHandle>(this, id);

		_slots.push_back(std::make_unique<Slot>(Slot{ handle, std::forward<Fn>(fn)}));
		_slotMap.insert({ id, _slots.back().get()});

		return handle;
	}

	template<typename ...Ts>
	void Event<Ts...>::Unsubscribe(HandleId id)
	{
		if (auto it = _slotMap.find(id); it != _slotMap.end())
		{
			std::erase_if(_slots, [it](const std::unique_ptr<Slot>& slt) { return slt->_eventHandle == it->second->_eventHandle; });
			_slotMap.erase(id);
		}
	}

	template<typename ...Ts>
	template<typename ...Us>
	inline void sw::core::Event<Ts...>::Slot::Invoke(Us && ...args)
	{
		_fn(std::forward<Us>(args)...);
	}
}
