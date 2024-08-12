#include "AggressiveWavesEffect.h"
#include "D2DGameEngine/BitmapComponent.h"

AggressiveWavesEffect::AggressiveWavesEffect(World* _world) : Projectile(_world)
{
	Inactivate();
}

AggressiveWavesEffect::~AggressiveWavesEffect()
{
}

void AggressiveWavesEffect::BeginPlay()
{
	__super::BeginPlay();
}

void AggressiveWavesEffect::Update(float _dt)
{
	__super::Update(_dt);
}
