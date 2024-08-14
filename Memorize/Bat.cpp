#include "Bat.h"

#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/CircleComponent.h"
#include "D2DGameEngine/World.h"

#include "MovementComponent.h"
#include "Player.h"

Bat::Bat(World* _world) : BossSkillActor(_world)
{
	ReflectionIn();

	bm->isVisible = false;

	CircleComponent* circle = CreateComponent<CircleComponent>();
	circle->collisionProperty = CollisionProperty(CollisionPropertyPreset::DynamicOverlapAll);
	circle->bGenerateOverlapEvent = true;
	rootComponent = circle;

	//rootComponent->AddChild(mv);

	moveAnimation = CreateComponent<AnimationBitmapComponent>();
	moveAnimation->SetSprite(L"TestResource/Boss/Bat/Idle/Bat_Idle.png");
	moveAnimation->SliceSpriteSheet(32, 32, 0, 0, 0, 0);
	moveAnimation->SetFrameDurations({ 0.125 });
	moveAnimation->isVisible = true;
	moveAnimation->SetLoop(true);
	rootComponent->AddChild(moveAnimation);

	attackAnimation = CreateComponent<AnimationBitmapComponent>();
	attackAnimation->SetSprite(L"TestResource/Boss/Bat/Attack/Bat_Attack.png");
	attackAnimation->SliceSpriteSheet(32, 32, 0, 0, 0, 0);
	attackAnimation->SetFrameDurations({ 0.125 });
	attackAnimation->isVisible = false;
	attackAnimation->SetLoop(true);
	rootComponent->AddChild(attackAnimation);

	DamageType damageType{
		.damageImpulse = 10000.f,
	};

	batDamageEvent.damageType = damageType;
	batDamageEvent.damage = 5.f;
}

void Bat::BeginPlay()
{
	Super::BeginPlay();

	moveAnimation->Trigger(true);
	player = GetWorld()->FindActorByType<Player>();
}

void Bat::OnBeginOverlap(Actor* other)
{
	// 스킬 쳐맞으면 사망띠
}

void Bat::Update(float _dt)
{
	static float elapsedTime = 0.f;
	Super::Update(_dt);

	Math::Vector2 playerLocation = player->GetLocation();
	Math::Vector2 toPlayer = playerLocation - GetLocation();
	float distanceToPlayerSquared = toPlayer.LengthSquared();
	toPlayer.Normalize();

	if (toPlayer.x >= 0) {
		moveAnimation->FlipX(false);
		attackAnimation->FlipX(false);
	}
	else {
		moveAnimation->FlipX(true);
		attackAnimation->FlipX(true);
	}

	if (attackAnimation->IsPlaying()) {
		elapsedTime += _dt;
		if (elapsedTime >= 1.f) {
			
			batDamageEvent.shotDirection = toPlayer;
			batDamageEvent.hitInfo.hitComponent = (PrimitiveComponent*) player->rootComponent;
			player->TakeDamage(5.f, batDamageEvent, nullptr, this);
			
			if (distanceToPlayerSquared > attackRange * attackRange)
			{
				// 어택 애니메이션 끄고 무브
				moveAnimation->Trigger(true);
				moveAnimation->isVisible = true;

				attackAnimation->Trigger(false);
				attackAnimation->isVisible = false;

				elapsedTime = 0.f;
			}
			else
			{
				elapsedTime -= 1.f;
			}
		}
	}
	else {
		if (distanceToPlayerSquared <= attackRange * attackRange)
		{
			// 어택 애니메이션 돌리고 
			moveAnimation->Trigger(false);
			moveAnimation->isVisible = false;

			attackAnimation->Trigger(true);
			attackAnimation->isVisible = true;
		}
		else
		{
			AddVelocity(toPlayer * speed);
		}
	}
}

void Bat::ReflectionIn()
{
	//auto reflectionResource = ResourceManager::LoadResource<ReflectionResource>();
}
