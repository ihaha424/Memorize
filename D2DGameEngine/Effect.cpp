#include "framework.h"
#include "Effect.h"
#include "ResourceManager.h"

EffectResource::EffectResource()
{
	if (resource)
	{
		resource->Release();
		resource = nullptr;
	}
}

EffectResource::~EffectResource()
{
}

void EffectResource::LoadFile(std::wstring _key)
{
}
