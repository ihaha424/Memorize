#include "TighteningCircle.h"
#include "TighteningCircleEffect.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/CapsuleComponent.h"
#include "D2DGameEngine/Mouse.h"

TighteningCircle::TighteningCircle(Actor* _owner) : RangeSkill(_owner)
{
	SetTickProperties(TICK_UPDATE);
	SetID(ST_RANGE, SE_DARKNESS);

	//�� ȿ�� ���� ����
	circleEffect = GetWorld()->GetCurLevel()->CreateActor<TighteningCircleEffect>();
	
}

TighteningCircle::~TighteningCircle()
{
}

void TighteningCircle::UseSkill()
{
	circleEffect->Activate();

	Math::Vector2 attackPos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	attackPos = GetWorld()->ScreenToWorldPoint(attackPos);
	circleEffect->SetLocation(attackPos.x, attackPos.y);
	circleEffect->GetComponent<BitmapComponent>()->SetScale(1.f, 1.f);
	circleEffect->Initialize();
}