#include "TighteningCircle.h"
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/CapsuleComponent.h"
#include "D2DGameEngine/Mouse.h"

TighteningCircle::TighteningCircle(Actor* _owner) : RangeSkill(_owner)
{
	SetTickProperties(TICK_UPDATE);
	SetID(ST_RANGE, SE_DARKNESS);

	//원 효과 액터 생성
	circle = GetWorld()->GetCurLevel()->CreateActor<Actor>();
	circle->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	circleBitmap = circle->CreateComponent<BitmapComponent>();
	circle->rootComponent = circleBitmap;
	//bm->SetSprite(L"TestResource/Skill/Range/TighteningCircle.png");

	//충돌 범위 
	circleShape = circle->CreateComponent<CapsuleComponent>();
	circleShape->SetCapsuleRadius(circleBitmap->GetSpriteWidth() / 2);
	circleShape->SetCapsuleHalfHeight(circleBitmap->GetSpriteHeight() / 2);
	circle->rootComponent->AddChild(circleShape);
	//TODO: 충돌 관련 설정 및 처리 
}

TighteningCircle::~TighteningCircle()
{
}

void TighteningCircle::BeginPlay()
{
	__super::BeginPlay();
	circle->Inactivate();
}

void TighteningCircle::Update(float _dt)
{
	elapsedTime += _dt;
	if (elapsedTime < tighteningTime)
	{
		float scale = (tighteningTime - elapsedTime) / tighteningTime;
		circleBitmap->SetScale(scale, scale);
	}

}

void TighteningCircle::UseSkill()
{
	circle->Activate();

	Math::Vector2 attackPos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	attackPos = GetWorld()->ScreenToWorldPoint(attackPos);
	circle->SetLocation(attackPos.x, attackPos.y);
	circle->GetComponent<BitmapComponent>()->SetScale(1.f, 1.f);
	elapsedTime = 0.f;
}
