#include "TighteningCircle.h"
#include "TighteningCircleEffect.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/CapsuleComponent.h"
#include "D2DGameEngine/Mouse.h"

TighteningCircle::TighteningCircle(Actor* _owner) : RangeSkill(_owner)
{
	ReflectionIn();
	SetID(ST_RANGE, SE_DARKNESS);

	//원 효과 액터 생성
	circleEffect = GetWorld()->GetCurLevel()->CreateActor<TighteningCircleEffect>();
	circleEffect->SetSkillID(id);
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
	circleEffect->SetDuration(2.f);
	circleEffect->SetDamage(damage);
	circleEffect->Initialize();
	SoundManager::PlayMusic(L"TestResource/Sound/Player/Sound_TighteningCircle.wav");
}

void TighteningCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/TighteningCircle.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, conditionCount, castingTime, bCrash, text);
}
