#include "Fireball.h"
#include "FireballProjectile.h"
#include "Player.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/World.h"
#include "D2dGameEngine/ResourceManager.h"
#include "MagicCircle.h"

Fireball::Fireball(Actor* _owner) : ProjectileSkill(_owner)
{
	SetID(ST_PROJECTILE, SE_FIRE);
	ReflectionIn();
	projectileCount = 5;

	for (int i = 0; i < projectileCount; i++)
	{
		projectiles.push_back(GetWorld()->GetCurLevel()->CreateActor<FireballProjectile>());
		projectiles[i]->SetVelocity({ 0,0 }, 0);
		projectiles[i]->SetDamage(damage);
		projectiles[i]->SetSpeed(projectileSpeed); 
		projectiles[i]->SetDuration(projectileDuration);
	}
	skillDuration = projectileDuration;
}

Fireball::~Fireball()
{
}


void Fireball::UseSkill()
{
	__super::UseSkill();
	//파이어볼 첫 위치 지정
	Projectile* fireball = projectiles[nowUsingCount];
	fireball->SetPlayer(player);
	MagicCircle* mc = player->GetComponent<MagicCircle>();
	fireball->SetLocation(mc->GetComponentLocation().x, mc->GetComponentLocation().y);
	fireball->SetStatus(OS_ACTIVE);
	fireball->Initialize();

	//마우스 위치로 이동시킴
	fireball->SetVelocity(attackDir, projectileSpeed);
	nowUsingCount = (nowUsingCount + 1 ) % projectileCount;

	//방향에 맞게 회전
	double rotateRad = std::acos(attackDir.Dot(Math::Vector2(1.f, 0.f)));
	if (attackDir.y < 0)
		rotateRad *= -1;
	fireball->rootComponent->SetRotation(rotateRad * 180.f / PI - 90);
	SoundManager::PlayMusic(L"TestResource/Sound/Player/Skill/Sound_Fireball01.wav");
}

void Fireball::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/Fireball.txt");
	reflectionResource->ParsingFile(0, damage, mana, commandList, projectileSpeed,
		projectileDuration, conditionCount, castingTime, bCrash, text);
}

