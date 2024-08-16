#include "Boss.h"
#include "BossHPPanel.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"

Boss::Boss(World* _world) : Character(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	hp = maxHp;

	hpBar = GetWorld()->GetCanvas()->CreatePannel<BossHPPanel>(L"BossHpBar");

}

Boss::~Boss()
{
}

void Boss::Update(float _dt)
{
	__super::Update(_dt);

	hpBar->SetValue(hp / (float)maxHp);
}
