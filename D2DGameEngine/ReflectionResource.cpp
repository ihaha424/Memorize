#include "framework.h"
#include "ReflectionResource.h"

const std::type_index ReflectionResource::TYPE_ID = std::type_index(typeid(ReflectionResource));

ReflectionResource::ReflectionResource()
{
	resource = new ReflectionValue();
}

ReflectionResource::~ReflectionResource()
{
	delete resource;
}

void ReflectionResource::LoadFile(std::wstring _key)
{
	std::wifstream file(_key);
	if (!file.is_open())
	{
		OBJ_ERROR(-1, "Reflection File Load Fail.");
		return;
	}

	std::wstring line;
	std::getline(file, line);
	while (!file.eof())
	{
		std::wstring valueName;
		std::wstring value;
		std::wstringstream wss(line);
		{
			getline(wss, valueName, L':');
			getline(wss, value, L':');
		}
		resource->push_back(ReflectionData(valueName, value));

		std::getline(file, line);
	}
}