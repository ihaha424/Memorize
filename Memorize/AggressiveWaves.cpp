#include "AggressiveWaves.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "Player.h"
#include "AggressiveWavesEffect.h"

AggressiveWaves::AggressiveWaves(Actor* _owner) : RangeSkill(_owner)
{
	SetID(ST_RANGE, SE_WATER);
	ReflectionIn();

	for (int i = 0; i < 8; i++)
	{
		waveEffects.push_back(GetWorld()->GetCurLevel()->CreateActor<AggressiveWavesEffect>());
	}
	
}

AggressiveWaves::~AggressiveWaves()
{
}

void AggressiveWaves::UseSkill()
{
	for (int i = 0; i < 4; i++)
	{
		waveEffects[curIndex + i]->Activate();
		waveEffects[curIndex + i]->SetLocation(player->GetLocation().x, player->GetLocation().y);
		
		float radians = angles[i] * PI / 180.f;
		Math::Vector2 newDir ={ std::cos(radians),std::sin(radians)};
		waveEffects[curIndex + i]->SetVelocity(newDir, speed);
		waveEffects[curIndex + i]->SetDamage(damage);
		waveEffects[curIndex + i]->SetSkillID(id);
		waveEffects[curIndex + i]->SetAinimation();
	}
	for (int i = 0; i < 4; i++)
	{
		waveEffects[curIndex + i]->SetRotation(SetAngles[i]);
	}
	curIndex = (curIndex + 4) % 8;
}

void AggressiveWaves::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/AggressiveWaves.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, conditionCount, castingTime, bCrash, text);
}

