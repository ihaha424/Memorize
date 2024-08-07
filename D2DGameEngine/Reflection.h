#pragma once

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
	void operator() (T& _value, const std::wstring _filevalue)
	{
		ReflectInValue(_value, _filevalue);
	}
private:
	//Default int
	template<typename T>
	static void ReflectInValue(T& _value, const std::wstring& Filevalue) { _value = _wtoi(Filevalue.c_str()); }
	template<>
	static void ReflectInValue(std::wstring& _value, const std::wstring& Filevalue) { _value = Filevalue; }
	template<>
	static void ReflectInValue(double& _value, const std::wstring& Filevalue) { _value = _wtof(Filevalue.c_str()); }
	template<>
	static void ReflectInValue(float& _value, const std::wstring& Filevalue) { _value = _wtof(Filevalue.c_str()); }
	template<>
	static void ReflectInValue(int& _value, const std::wstring& Filevalue) { _value = _wtoi(Filevalue.c_str()); }
	template<>
	static void ReflectInValue(char& _value, const std::wstring& Filevalue) { _value = Filevalue[0]; }
	template<>
	static void ReflectInValue(std::vector<int>& _value, const std::wstring& Filevalue)
	{
		std::wstringstream wss(Filevalue);
		std::wstring value;
		while (getline(wss, value, L','))
			_value.push_back(_wtoi(value.c_str()));
	}
	template<>
	static void ReflectInValue(std::vector<float>& _value, const std::wstring& Filevalue)
	{
		std::wstringstream wss(Filevalue);
		std::wstring value;
		while (getline(wss, value, L','))
			_value.push_back(_wtof(value.c_str()));
	}
};

class ReflectionOut
{
public:

};
