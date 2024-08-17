#include "LightStreamEffect.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/DamageEvent.h"

LightStreamEffect::LightStreamEffect(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	rootComponent = bm = CreateComponent<BitmapComponent>();
	bm->SetSprite(L"TestResource/Skill/Range/LightStream.png");

	box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	box->bSimulatePhysics = false;	// �����ӿ� ������ �������� �ʽ��ϴ�.
	box->bApplyImpulseOnDamage = false;	// �������� ���� �� ����� ���մϴ�.
	box->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	rootComponent->AddChild(box);
}
void LightStreamEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void LightStreamEffect::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);

	if (box->bGenerateOverlapEvent == false)
		return;

	//������� ����
	DamageEvent damageEvent;
	DamageType damageType{
		.damageImpulse = 10000.f, //��ݷ� �־��ִ� �� �´���?
	};
	damageEvent.SetDamageType(damageType);

	other->TakeDamage(damage, damageEvent, nullptr, this);
}

void LightStreamEffect::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;
	if (elapsedTime > duration)
	{
		elapsedTime = 0.f;
		Inactivate();
	}
}

LightStreamEffect::~LightStreamEffect()
{
}
