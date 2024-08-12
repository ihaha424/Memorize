 #include "Boss.h"

Boss::Boss(World* _world) : Character(_world)
{
	hp = maxHp;
}

Boss::~Boss()
{
}

void Boss::Update(float _dt)
{
	__super::Update(_dt);
}
