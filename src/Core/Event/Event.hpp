#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <memory>
#include <functional>
#include <limits>

namespace sw::core
{
	using HandleId = size_t;

	struct IEvent
	{
		virtual void Unsubscribe(HandleId id) = 0;
		virtual ~IEvent() noexcept = default;
	};

	struct EventHandle;

	template<typename... Ts>
	struct Event : IEvent
	{
		Event();
		~Event();

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

		void Invoke(Ts... args);

		template<typename Fn>
		std::shared_ptr<EventHandle> Subscribe(Fn&& fn);

		void Unsubscribe(HandleId id);

	private:
		struct Slot
		{
			std::shared_ptr<EventHandle> _eventHandle = nullptr;
			std::function<void(Ts...)> _fn;

			template<typename... Us>
			void Invoke(Us&&... args);
		};

		using SlotContainerT = std::vector<std::unique_ptr<Slot>>;

		SlotContainerT _slots;
		std::unordered_map<HandleId, Slot*> _slotMap;
	};

	struct EventHandle
	{
		template<typename... Ts>
		friend struct Event;

		EventHandle(IEvent* event, HandleId id) noexcept;

		void Unsubscribe() noexcept;
		~EventHandle() noexcept;

		HandleId GetHandleId() const noexcept;

	protected:
		IEvent* _event;
		HandleId _id;
	};
}

#include "Event.inl"
