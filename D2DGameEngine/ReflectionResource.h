#pragma once
#include "Resource.h"
#include "Debug.h"
#include "Reflection.h"

using ReflectionData = std::pair<std::wstring, std::wstring>;
using ReflectionValue = std::vector<ReflectionData>;

/**
 * @brief ReflectionResouce
 * FileFormat
 * (Name?)
 * ValueName1: Value1
 * ValueName2: Value2
 * ...
 */
class ReflectionResource : public Resource<ReflectionResource, ReflectionValue>
{
public:
	ReflectionResource() {};
	~ReflectionResource();

	void LoadFile(std::wstring _key);

	static const std::type_index TYPE_ID;
};

