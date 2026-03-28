#pragma once

#include "DataPack.hpp"

namespace sw
{
	template<core::DataConcept ...Ds>
	inline DataPack::DataPack(Ds&& ...data)
		: _data()
	{
		(Put<Ds>(std::forward<Ds>(data)), ...);
	}

	template<core::DataConcept D>
	void DataPack::Put(D&& in)
	{
		auto id = GetDataId<D>();

		auto [it, result] = _data.try_emplace(id, nullptr);
		if (!result)
		{
			assert(false && "Can't emplace new data into container");
			return;
		}

		it->second = std::make_unique<D>(std::forward<D>(in));
	}

	template<core::DataConcept D>
	D* DataPack::TryTake()
	{
		auto id = GetDataId<D>();

		if (auto it = _data.find(id); it != _data.end())
		{
			return static_cast<D*>(it->second.get());
		}

		return nullptr;
	}

	template<typename T>
	DataPack::DataId DataPack::GetDataId() noexcept
	{
		static DataId id = _nextDataId++;
		return id;
	}
}
