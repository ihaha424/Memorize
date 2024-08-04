#pragma once

#include "framework.h"

#include "Types.h"

#include "Descriptor.h"

class ObjectRegistryID {
	static Descriptor64_t _type_id;

	template<typename T>
	class ObjectRegistry {
		Descriptor64_t _id{ 0ULL };
	public:
		ObjectRegistry() { ++_type_id.version; }

		Descriptor64_t index() {
			return _id.index++;
		}
	};

public:
	template<typename T>
	static object_id Get() {
		static ObjectRegistry<T> obj_reg{};
		return _type_id | obj_reg.index();
	}
};

inline
Descriptor64_t ObjectRegistryID::_type_id{ 0ULL };
