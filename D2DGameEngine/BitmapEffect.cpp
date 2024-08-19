#include "BitmapEffect.h"
#include "BitmapComponent.h"

BitmapEffect::BitmapEffect(World* _world)
	: IEffect(_world)
{
	bitmap = CreateComponent<BitmapComponent>();
}
