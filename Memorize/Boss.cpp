#include "Boss.h"

Boss::Boss(World* _world) : Character(_world)
{
	hp = maxHp;
}

Boss::~Boss()
{
}
