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

	for (int i = 0; i < 4; i++)
	{
		waveEffects.push_back(GetWorld()->GetCurLevel()->CreateActor<AggressiveWavesEffect>());
	}
	waveEffects[0]->GetComponent<BitmapComponent>()->SetSprite(L"TestResource/Skill/Range/AggressiveWaves.png");
	waveEffects[1]->GetComponent<BitmapComponent>()->SetSprite(L"TestResource/Skill/Range/AggressiveWaves2.png");
	waveEffects[2]->GetComponent<BitmapComponent>()->SetSprite(L"TestResource/Skill/Range/AggressiveWaves.png");
	waveEffects[3]->GetComponent<BitmapComponent>()->SetSprite(L"TestResource/Skill/Range/AggressiveWaves2.png");
}

AggressiveWaves::~AggressiveWaves()
{
}

void AggressiveWaves::UseSkill()
{
	for (int i = 0; i < 4; i++)
	{
		waveEffects[i]->Activate();
		waveEffects[i]->SetLocation(player->GetLocation().x, player->GetLocation().y);
		
		float radians = angles[i] * PI / 180.f;
		Math::Vector2 newDir ={ std::cos(radians),std::sin(radians)};
		waveEffects[i]->SetVelocity(newDir, speed);
	}
	waveEffects[0]->SetRotation(0);
	waveEffects[1]->SetRotation(0);
	waveEffects[2]->SetRotation(180);
	waveEffects[3]->SetRotation(180);
}

void AggressiveWaves::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/AggressiveWaves.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, conditionCount, castingTime, bCrash, text);
}

