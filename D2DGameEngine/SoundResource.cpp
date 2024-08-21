#include "framework.h"
#include "SoundResource.h"
#include "SoundManager.h"
#include "Debug.h"

const std::type_index SoundResource::TYPE_ID = std::type_index(typeid(SoundResource));

SoundResource::~SoundResource()
{
	//if (resource)
	//{
	//	resource->Release();
	//	resource = nullptr;
	//}
}

void SoundResource::LoadFile(std::wstring _key)
{
	// Do Nothing
	// If this Excute -> Error
}

void SoundResource::LoadFileLoop(std::wstring _key, bool _loopCheck)
{
	SoundManager::CreateSound(_key, _loopCheck, resource);
}
