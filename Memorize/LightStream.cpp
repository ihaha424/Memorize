#include "LightStream.h"
#include "LightStreamEffect.h"
#include "GPlayerController.h"
#include "PlayerFSMComponent.h"
#include "PlayerAttack.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/AnimationEffect.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/Mouse.h"
#include "Player.h"
#include "MagicCircle.h"
#include "TutorialPlayer.h"

LightStream::LightStream(Actor* _owner) : RangeSkill(_owner)
{
	SetID(ST_RANGE, SE_LIGHT);
	ReflectionIn();
	for (int i = 0; i < 4; i++)
	{
		lightStreamEffect[i] = GetWorld()->GetCurLevel()->CreateActor<LightStreamEffect>();
		lightStreamEffect[i]->SetDamage(damage);
		lightStreamEffect[i]->SetSkillID(id);
	}
	mana = -1;


}

LightStream::~LightStream()
{
}

void LightStream::UseSkill()
{
	__super::UseSkill();

	lightStreamEffect[curIndex]->Activate();
	lightStreamEffect[curIndex]->SetSkillID(id);

	lightStreamEffect[curIndex]->SetDuration(mana / 50.f);

	//마우스 위치 
	Math::Vector2 attackPos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	attackPos = GetWorld()->ScreenToWorldPoint(attackPos);
	//마우스 방향
	Math::Vector2 direction = attackPos - player->GetLocation();
	direction.Normalize();

	//플레이어에서 마우스 방향으로 위치 조정
	float halfLength = 500.f;
	MagicCircle* mc = player->GetComponent<MagicCircle>();
	lightStreamEffect[curIndex]->SetLocation(mc->GetComponentLocation().x + direction.x * halfLength, mc->GetComponentLocation().y + direction.y * halfLength);
	
	//방향에 맞게 회전
	float rotateRad = std::acos(direction.Dot(Math::Vector2(1.f, 0.f)));
	if (direction.y < 0) rotateRad *= -1;
	lightStreamEffect[curIndex]->rootComponent->SetRotation(rotateRad * 180.f / PI);

	lightStreamEffect[curIndex]->Initialize();

	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_LightStream.wav");

	//Orb Effect
	{
		orbEffect = controller->GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		orbEffect->SetSprite(L"TestResource/Player/Orb/Orb.png");
		orbEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(88, 100, 0, 0, 0, 0);
		orbEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 2.f / 13 });
		orbEffect->GetAnimationBitmapComponent()->Trigger(true);
		orbEffect->GetAnimationBitmapComponent()->SetOpacity(0.5f);
		orbEffect->SetAliveTime(mana / 50.f + 1);
		orbEffect->SetLocation(player->GetLocation().x + direction.x * 80, player->GetLocation().y + direction.y * 80);
		orbEffect->GetAnimationBitmapComponent()->FlipX(direction.x > 0 ? true : false);
		
		player->orb->SetStatus(OS_ACTIVE);
	}

	curIndex = (curIndex + 1) % maxIndex;

}

void LightStream::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/LightStream.txt");
	reflectionResource->ParsingFile(0, damage, commandList, conditionCount, castingTime, bCrash, text);
}
