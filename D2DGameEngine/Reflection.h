#pragma once

#include "Debug.h"

class IReflection
{
public:
	virtual void ReflectionIn() = 0;
	virtual void ReflectionOut() = 0;
};

class ReflectionIn
{
public:
	template<typename T>
	void operator() (T& value, std::wstring _valueName)
	{
		ReflectValue(value, _valueName);
	}
	template<typename T>
	static void ReflectInValue(T& _value, std::wstring& Filevalue) {}
	template<>
	static void ReflectInValue(std::wstring& _value, std::wstring& Filevalue) { _value = Filevalue; }
	template<>
	static void ReflectInValue(double& _value, std::wstring& Filevalue) { _value = _wtof(Filevalue.c_str()); }
	template<>
	static void ReflectInValue(float& _value, std::wstring& Filevalue) { _value = _wtof(Filevalue.c_str()); }
	template<>
	static void ReflectInValue(int& _value, std::wstring& Filevalue) { _value = _wtoi(Filevalue.c_str()); }
	template<>
	static void ReflectInValue(char& _value, std::wstring& Filevalue) { _value = Filevalue[0]; }
};

class ReflectionOut
{
public:

};

/**
 * @brief ReflectionResouce
 * FileFormat
 * (Name?)
 * ValueName1: Value1
 * ValueName2: Value2
 * ...
 */
class ReflectionResouce
{
	LOG_REGISTER_OBJ(ReflectionResouce)
									//	ValueName,		Value
	using ReflectionValue = std::pair<std::wstring, std::wstring>;
public:
	template<typename... ARGS>
	void Value(ARGS&... _values)
	{
		int index = 0;
		for (auto value : _values)
		{
			if(index == values.size())
				OBJ_ERROR(-1, "Reflection Argument is Too Large.")
			ReflectionIn(value, values[index++].second);
		}
		if (index < values.size())
			OBJ_ERROR(-1, "Reflection Argument is Too Small.")
	}

	template<typename T>
	void SaveValue(std::wstring _valueName, T _value)
	{
		// 아직 필요없을 것 같음
	}


	void LoadValue(std::wstring _filePath)
	{
		std::wifstream file(_filePath);
		if (!file.is_open())
		{
			OBJ_ERROR(-1, "Reflection File Load Fail.")
		}

		std::wstring line;
		std::wstring valueName;
		std::wstring value;
		while (!file.eof())
		{
			std::getline(file, line);
			std::wstringstream wss(line);
			{
				getline(wss, valueName, L':');
				getline(wss, value, L':');
			}
			values.push_back(ReflectionValue(valueName, value));

			std::getline(file, line);
		}
	}

private:
	std::vector<ReflectionValue> values;
};