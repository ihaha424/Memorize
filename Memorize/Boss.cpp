#include "Boss.h"
#include "Signal.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"

Boss::Boss(World* _world) : Character(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	hp = maxHp;


}

Boss::~Boss()
{
	delete OnHPChanged;
}

void Boss::Update(float _dt)
{
	__super::Update(_dt);

	OnHPChanged->Emit(hp);
}
