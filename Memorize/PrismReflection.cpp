#include "PrismReflection.h"
#include "PrismReflectionProjectile.h"
#include "../D2DGameEngine/World.h"
#include "D2DGameEngine/RandomGenerator.h"
#include "MovementComponent.h"
#include "Player.h"
#include "D2dGameEngine/ResourceManager.h"

PrismReflection::PrismReflection(Actor* _owner) : ProjectileSkill(_owner)
{
	projectileCount = 20;
	SetID(ST_PROJECTILE, SE_LIGHT);
	ReflectionIn();
	projectileDuration = 1.f;

	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<PrismReflectionProjectile>());
		projectiles[i]->SetVelocity({ 0,0 }, 0);
		projectiles[i]->SetDamage(damage);
		projectiles[i]->SetDuration(projectileDuration);
		projectiles[i]->SetSpeed(projectileSpeed); 
		projectiles[i]->SetSkillID(id);
	}
	
	skillDuration = projectileDuration;
}

PrismReflection::~PrismReflection()
{
}
void PrismReflection::UseSkill()
{
	__super::UseSkill();


	//시전 방향 기준 5도 간격 9방향 체크 
	std::vector<Math::Vector2> directions;
	for (int i = -4; i <= 4; ++i)
	{
		float radians = i * 5 * PI / 180.f;
		Math::Vector2 newDir =
		{
			attackDir.x * std::cos(radians) - attackDir.y * std::sin(radians),
			attackDir.x * std::sin(radians) + attackDir.y * std::cos(radians)
		};
		directions.push_back(newDir);
	}
	
	//랜덤으로 N 방향에 투사체 발사 
	for (int i = 0; i < 10; i++)
	{
		int n = Random::Get<int>(8);
		Projectile* nowPj = projectiles[nowUsingCount];
		nowPj->SetDelay(0.1f * i);
		nowPj->SetLocation(player->GetLocation().x, player->GetLocation().y);
		nowPj->SetVelocity(directions[n], projectileSpeed);
		nowPj->Activate();
		nowUsingCount = (nowUsingCount + 1) % projectileCount;
	}

	SoundManager::PlayMusic(L"TestResource/Sound/Player/Sound_PrismReflection01.wav");
}

void PrismReflection::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PrismReflection.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, projectileSpeed, skillDuration, conditionCount, castingTime, bCrash, text);
}
