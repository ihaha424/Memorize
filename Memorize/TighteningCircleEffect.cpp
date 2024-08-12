#include "TighteningCircleEffect.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/CapsuleComponent.h"

TighteningCircleEffect::TighteningCircleEffect(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_UPDATE | TICK_RENDER);
	bitmap = CreateComponent<BitmapComponent>();
	rootComponent = bitmap;
	//bm->SetSprite(L"TestResource/Skill/Range/TighteningCircle.png");

	//�浹 ���� 
	capsule = CreateComponent<CapsuleComponent>();
	capsule->SetCapsuleRadius(bitmap->GetSpriteWidth() / 2);
	capsule->SetCapsuleHalfHeight(bitmap->GetSpriteHeight() / 2);
	rootComponent->AddChild(capsule);
	//TODO: �浹 ���� ���� �� ó�� 
}

TighteningCircleEffect::~TighteningCircleEffect()
{
}

void TighteningCircleEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void TighteningCircleEffect::Update(float _dt)
{
	__super::Update(_dt);
	elapsedTime += _dt;
	if (elapsedTime < tighteningTime)
	{
		
		float scale = (tighteningTime - elapsedTime) / tighteningTime;
		bitmap->SetScale(scale, scale);
	}
}

void TighteningCircleEffect::Initialize()
{
	elapsedTime = 0.f;
}
