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
	LOG_REGISTER_OBJ(ReflectionResource)
public:
	ReflectionResource();
	~ReflectionResource();

	void LoadFile(std::wstring _key);
	
	template<typename T>
	void ParsingFile(int _index, T& _value)
	{
		if ((*resource).size() <= _index)
			OBJ_ERROR(-1, "Reflection Data is Too Many Data.");
		std::wstring tmep((*resource)[_index].second);
		ReflectionIn()(_value, tmep);
	}

	template<typename T, typename... ARGS>
	void ParsingFile(int _index, T& _value, ARGS&... _values)
	{
		if ((*resource).size() <= _index)
			OBJ_ERROR(-1, "Reflection Data is Too Many Data.");
		std::wstring tmep((*resource)[_index].second);
		ReflectionIn()(_value, tmep);
		ParsingFile(++_index, _values...);
	}

	static const std::type_index TYPE_ID;
};

