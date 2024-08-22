#include "LightStream.h"
#include "LightStreamEffect.h"
#include "GPlayerController.h"
#include "PlayerFSMComponent.h"
#include "PlayerAttack.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/Mouse.h"
#include "Player.h"
#include "MagicCircle.h"

LightStream::LightStream(Actor* _owner) : RangeSkill(_owner)
{
	SetID(ST_RANGE, SE_LIGHT);
	ReflectionIn();
	lightStreamEffect = GetWorld()->GetCurLevel()->CreateActor<LightStreamEffect>();
	lightStreamEffect->SetDamage(damage);
	lightStreamEffect->SetSkillID(id);
	mana = -1;

}

LightStream::~LightStream()
{
}

void LightStream::UseSkill()
{
	lightStreamEffect->Activate();
	lightStreamEffect->SetSkillID(id);

	lightStreamEffect->SetDuration(mana / 50.f);

	//마우스 위치 
	Math::Vector2 attackPos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	attackPos = GetWorld()->ScreenToWorldPoint(attackPos);
	//마우스 방향
	Math::Vector2 direction = attackPos - player->GetLocation();
	direction.Normalize();

	//플레이어에서 마우스 방향으로 위치 조정
	float halfLength = 500.f;
	MagicCircle* mc = player->GetComponent<MagicCircle>();
	lightStreamEffect->SetLocation(mc->GetComponentLocation().x + direction.x * halfLength, mc->GetComponentLocation().y + direction.y * halfLength);
	
	//방향에 맞게 회전
	float rotateRad = std::acos(direction.Dot(Math::Vector2(1.f, 0.f)));
	if (direction.y < 0) rotateRad *= -1;
	lightStreamEffect->rootComponent->SetRotation(rotateRad * 180.f / PI);

	lightStreamEffect->Initialize();

	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_LightStream.wav");
	controller->FindCurSkiil()->castingTime = mana / 50.f + 1.f;
}

void LightStream::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/LightStream.txt");
	reflectionResource->ParsingFile(0, damage, commandList, conditionCount, castingTime, bCrash, text);
}
