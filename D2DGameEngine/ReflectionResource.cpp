#include "framework.h"
#include "ReflectionResource.h"

const std::type_index ReflectionResource::TYPE_ID = std::type_index(typeid(ReflectionResource));

ReflectionResource::~ReflectionResource()
{}

void ReflectionResource::LoadFile(std::wstring _key)
{
	std::wifstream file(_key);
	if (!file.is_open())
	{
		LOG_ERROR(-1, "Reflection File Load Fail.");
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
		resource->push_back(ReflectionData(valueName, value));

		std::getline(file, line);
	}
}