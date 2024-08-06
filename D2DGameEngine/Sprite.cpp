#include "framework.h"
#include "Sprite.h"
#include "ResourceManager.h"



const std::type_index SpriteResource::TYPE_ID = std::type_index(typeid(SpriteResource));

SpriteResource::~SpriteResource()
{
	if (resource)
	{
		resource->Release();
		resource = nullptr;
	}
}

void SpriteResource::LoadFile(std::wstring _key)
{
	resource = ResourceManager::CreateID2D1Bitmap(_key);
}
