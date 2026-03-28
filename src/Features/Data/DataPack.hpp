#pragma once

#include "Core.hpp"

#include <initializer_list>
#include <cstdint>
#include <unordered_map>
#include <memory>

namespace sw
{
	struct DataPack
	{
		template<core::DataConcept... Ds>
		DataPack(Ds&&... data);

		template<core::DataConcept D>
		void Put(D&& in);

		template<core::DataConcept D>
		D* TryTake();

	private:
		using DataId = uint32_t;
		static DataId _nextDataId;

		template<typename T>
		DataId GetDataId() noexcept;
		
		std::unordered_map<DataId, std::unique_ptr<core::IData>> _data;
	};
}

#include "DataPack.inl"
