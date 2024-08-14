#include "Pattern06.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/CircleComponent.h"

Pattern06::Pattern06(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();
	CreateComponent<ClickComponent>();

	circleComponent = CreateComponent<CircleComponent>();
	circleComponent->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);	// 오브젝트의 충돌 채널은 WorldStatic, 모든 충돌 채널에 대한 반응은 `Block`.
	circleComponent->bSimulatePhysics = false;				// 움직임에 물리를 적용합니다.
	circleComponent->bApplyImpulseOnDamage = true;	// 데미지를 받을 때 충격을 가합니다.
	circleComponent->bGenerateOverlapEvent = false;	// Overlap 이벤트를 발생시킵니다.
	circleComponent;	// 게임 오브젝트의 루트 컴포넌트가 충돌체 입니다.
	bm->AddChild(circleComponent);
}

void Pattern06::BeginPlay()
{
	__super::BeginPlay();
	bm->SetSprite(L"TestResource/Boss/MagicCircle/Pattern06_MagicCircle.png");
	bm->Translate(0, 200);
	circleComponent->InitCircleRadius(bm->GetSpriteHeight()/2);	// 반지름이 62이고 높이가 110 인 캡슐 충돌체를 초기화 합니다.
	circleComponent->SetStatus(EObjectStatus::OS_INACTIVE);
}

void Pattern06::Update(float _dt)
{
	if (skillDuration > 0.f)
	{
		bm->Scale(1.0f + _dt * 0.1f, 1.0f + _dt * 0.1f);
		skillDuration -= _dt;
	}
	if (skillDuration < 0.f)
	{
		circleComponent->SetStatus(EObjectStatus::OS_ACTIVE);
	}
}

void Pattern06::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern06_Actor.txt");
	int typeInt;
	reflectionResource->ParsingFile(0, isFragile, typeInt, dispelTime, skillDuration, damage);
	type = static_cast<BossSkillType>(typeInt);
}

void Pattern06::ReflectionOut() {}
