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
	hp -= damageAmount;	// ü���� ���� ������ ��ŭ ���ҽ�ŵ�ϴ�.

	if (hp <= 0.f)	// ���� ü���� 0���� �۰ų� ���ٸ�,
	{
		hp = 0.f;
		//TODO ��� �� ó�� 
	}
}