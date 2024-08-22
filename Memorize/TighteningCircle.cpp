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
	for (int i = 0; i < 2; i++)
	{
		circleEffects[i] = GetWorld()->GetCurLevel()->CreateActor<TighteningCircleEffect>();
		circleEffects[i]->SetSkillID(id);
	}

}

TighteningCircle::~TighteningCircle()
{
}

void TighteningCircle::UseSkill()
{
	
	circleEffects[nowUsingIndex]->Activate();

	Math::Vector2 attackPos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	attackPos = GetWorld()->ScreenToWorldPoint(attackPos);
	circleEffects[nowUsingIndex]->SetLocation(attackPos.x, attackPos.y);
	circleEffects[nowUsingIndex]->SetDuration(2.f);
	circleEffects[nowUsingIndex]->SetDamage(damage);
	circleEffects[nowUsingIndex]->Initialize();
	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_TighteningCircle.wav");

	nowUsingIndex = (nowUsingIndex + 1) % 2;
}

void TighteningCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/TighteningCircle.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, conditionCount, castingTime, bCrash, text);
}
