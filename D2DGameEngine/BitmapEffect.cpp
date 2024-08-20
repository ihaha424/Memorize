#include "BitmapEffect.h"
#include "BitmapComponent.h"

BitmapEffect::BitmapEffect(World* _world)
	: IEffect(_world)
{
	bitmap = CreateComponent<BitmapComponent>();
	rootComponent = bitmap;
}

void BitmapEffect::SetSprite(std::wstring _path)
{
	bitmap->SetSprite(_path.c_str());
}