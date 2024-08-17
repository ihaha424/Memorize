#include "Boss.h"
#include "Signal.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "../D2DGameEngine/BoxComponent.h"

Boss::Boss(World* _world) : Character(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	hp = maxHp;

	OnHPChanged = new Signal<int>;

	GetComponent<BoxComponent>()->SetCollisionObjectType(ECollisionChannel::Enemy);
}

Boss::~Boss()
{
	delete OnHPChanged;
}

void Boss::Update(float _dt)
{
	__super::Update(_dt);

	OnHPChanged->Emit(hp/(float)maxHp);
}

void Boss::OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser)
{
	hp -= damageAmount;	// 체력을 받은 데미지 만큼 감소시킵니다.

	if (hp <= 0.f)	// 만약 체력이 0보다 작거나 같다면,
	{
		hp = 0.f;
		//TODO 사망 시 처리 
	}
}