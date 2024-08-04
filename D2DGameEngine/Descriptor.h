// Descriptor.h
#pragma once

#include "framework.h"

#include "Types.h"

/**
	* @brief
	*
	* @tparam value_type
	* @tparam version_bits
	* @tparam index_bits
	* @ref https://github.com/tobias-stein/EntityComponentSystem/blob/master/EntityComponentSystem/include/ECS/util/Handle.h
	*/
template<
	typename base_value_type,
	std::size_t index_bits,
	std::size_t version_bits
>
	requires // The size of base_value_type must be greater than or equal to version
	requires {
	index_bits + version_bits <= sizeof(base_value_type) * CHAR_BIT;
}
union Descriptor {

public:
	using value_type = base_value_type;

	static constexpr std::size_t NUM_INDEX_BITS{ index_bits };
	static constexpr std::size_t NUM_VERSION_BITS{ version_bits };

	static constexpr value_type MIN_VERSION{ 0 };
	static constexpr value_type MAX_VERSION{ (1U << NUM_VERSION_BITS) - 2U };
	static constexpr value_type MAX_INDICES{ (1U << NUM_INDEX_BITS) - 2U };

	static constexpr value_type INVALID_HANDLE{ (std::numeric_limits<value_type>::max)() };

private:
	value_type _value;
public:
	struct {
		value_type index : NUM_INDEX_BITS;
		value_type version : NUM_VERSION_BITS;
	};

	Descriptor() {}
	Descriptor(value_type value) : _value{ value } {}
	Descriptor(value_type index, value_type version)
		: index{ index }, version{ version } {}

	inline operator value_type() const { return _value; }
};

/**
	* @brief Defines a 32 bit descriptor
	* Max. possible : 1048576
	* Max. possible versions until loop per handle : 4096
	*/
using Descriptor32_t = Descriptor<uint32_t, 12, 20>;

using Descriptor64_t =
std::conditional_t<
	is_64bit_system_v,
	Descriptor<uint64_t, 24, 40>,
	Descriptor32_t
>;

// template<typename T>
// concept DescriptorType = requires(T t) {
//     // Check if the static constexpr members exist and have the correct types
//     { T::NUM_INDEX_BITS } -> std::convertible_to<std::size_t>;
//     { T::NUM_VERSION_BITS } -> std::convertible_to<std::size_t>;
//     { T::MIN_VERSION } -> std::convertible_to<typename T::value_type>;
//     { T::MAX_VERSION } -> std::convertible_to<typename T::value_type>;
//     { T::MAX_INDICES } -> std::convertible_to<typename T::value_type>;
//     { T::INVALID_HANDLE } -> std::convertible_to<typename T::value_type>;
// };

template< class T, typename desc_type, std::size_t grow = 1024 >
class DescriptorTable {
	using Descriptor = desc_type;
	using value_type = typename Descriptor::value_type;

private:
	using TableEntry = std::pair<value_type, T*>;

	std::vector<TableEntry> _table;

	void GrowTable() {
		std::size_t oldSize = _table.size();
		if (oldSize >= Descriptor::MAX_INDICES) {
			throw std::runtime_error("DescriptorTable: Max table capacity reached!");
		}

		bool t = oldSize + grow < Descriptor::MAX_INDICES;
		std::size_t newSize = t * (oldSize + grow) + !t * Descriptor::MAX_INDICES;

		_table.resize(newSize);

		for (value_type i = oldSize; i < newSize; ++i) {
			_table[i] = TableEntry{ Descriptor::MIN_VERSION, nullptr };
		}
	}

	bool IsValidDescriptor(const Descriptor& desc) const {
		return desc.index < _table.size() && desc.version == _table[desc.index].first;
	}

public:
	DescriptorTable() { GrowTable(); }
	~DescriptorTable() {}

	Descriptor ClaimDescriptor(T* rawObj) {
		// TODO: Can be optimized with a mem queue.
		value_type i = 0;
		for (; i < _table.size(); ++i) {
			if (_table[i].second == nullptr) {
				_table[i].second = rawObj;
				value_type next_value = _table[i].first + 1;
				_table[i].first =
					(next_value > Descriptor::MAX_VERSION) ?
					Descriptor::MIN_VERSION : next_value;

				return Descriptor{ i, _table[i].first };
			}
		}

		GrowTable();
		_table[i].first = 1;
		_table[i].second = rawObj;

		return Descriptor{ i, _table[i].first };
	}

	void ReleaseDescriptor(Descriptor desc) {
		if (!IsValidDescriptor(desc))
			throw std::runtime_error("Invalid Descriptor!");

		_table[desc.index].second = nullptr;
	}

	bool IsExpired(Descriptor desc) const {
		return _table[desc.index].first != desc.version ||
			_table[desc.index].second == nullptr;
	}

	inline
		Descriptor operator[](value_type index) const {
		if (index >= _table.size())
			throw std::out_of_range("Descriptor index out of range!");
		return Descriptor{ index, _table[index].first };
	}

	inline
		T* operator[](Descriptor desc) {
		if (!IsValidDescriptor(desc))
			throw std::runtime_error("Invalid Descriptor!");

		return (_table[desc.index].first == desc.version ?
			_table[desc.index].second : nullptr);
	}
};

