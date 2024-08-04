// TypeGroupID.h
#pragma once

#include "framework.h"

#include "Types.h"

/**
	* @brief Generate type registry ID,
	* such that `T` is the parent type, and `U` is its child type.
	* @tparam T Parent type
	*/
template<class T>
class TypeRegistryID {
	// The ID of the parent type.
	static type_id _id;
public:
	/**
		* @brief Return the unique ID of a child type.
		* @tparam U Child type
		* @return The ID of the child type
		*/
	template<class U>
	static const type_id Get() {
		static const type_id STATIC_TYPE_ID{ _id++ };
		return STATIC_TYPE_ID;
	}
	/**
		* @brief Return the type ID of the parent type.
		* @return The ID of the parent type
		*/
	static const type_id Get() {
		return _id;
	}
};


