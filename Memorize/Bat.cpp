#include "Bat.h"

#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/CircleComponent.h"
#include "D2DGameEngine/World.h"

#include "MovementComponent.h"
#include "Player.h"

Bat::Bat(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_PRE_UPDATE | TICK_UPDATE | TICK_POST_UPDATE | TICK_RENDER);
	ReflectionIn();

	renderLayer = 2;

	CircleComponent* circle = CreateComponent<CircleComponent>();
	circle->collisionProperty = CollisionProperty(CollisionPropertyPreset::Enemy);
	circle->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	circle->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Overlap);
	circle->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Overlap);
	circle->SetCollisionObjectType(ECollisionChannel::Enemy);
	circle->bGenerateOverlapEvent = true;
	circle->SetCircleRadius(30);
	circle->maxSpeed = speed;
	rootComponent = circle;

	moveAnimation = CreateComponent<AnimationBitmapComponent>();
	moveAnimation->SetSprite(L"TestResource/Boss/Bat/Idle/bat_idle.png");
	moveAnimation->SliceSpriteSheet(130, 170, 0, 0, 0, 0);
	moveAnimation->SetFrameDurations({ 0.125 });
	moveAnimation->isVisible = true;
	moveAnimation->SetLoop(true);
	rootComponent->AddChild(moveAnimation);

	attackAnimation = CreateComponent<AnimationBitmapComponent>();
	attackAnimation->SetSprite(L"TestResource/Boss/Bat/Attack/bat_attack.png");
	attackAnimation->SliceSpriteSheet(130, 170, 0, 0, 0, 0);
	attackAnimation->SetFrameDurations({ 0.125 });
	attackAnimation->FrameResize(9);
	attackAnimation->isVisible = false;
	attackAnimation->SetLoop(false);
	rootComponent->AddChild(attackAnimation);

	DamageType damageType{
		.damageImpulse = 10000.f,
	};
	batDamageEvent.SetDamageType(damageType);
	batDamageEvent.damage = damage;
}

void Bat::BeginPlay()
{
	Super::BeginPlay();

	moveAnimation->Trigger(true);
	player = GetWorld()->FindActorByType<Player>();
}

void Bat::OnTakeDamage(float damageAmount, DamageEvent const& damageEvent, Controller* eventInstigator, Actor* damageCauser)
{
	hp -= damageAmount;

	if (hp <= 0.f)
	{
		Destroy();
	}
}

void Bat::Update(float _dt)
{
	static float elapsedTime = 0.50f;
	Super::Update(_dt);
	
	GetComponent<CircleComponent>()->bShouldOverlapTest = true;
	Math::Vector2 playerLocation = player->GetLocation();
	Math::Vector2 toPlayer = playerLocation - GetLocation();
	float distanceToPlayerSquared = toPlayer.LengthSquared();
	toPlayer.Normalize();

	if (toPlayer.x >= 0) 
	{
		moveAnimation->FlipX(false);
		attackAnimation->FlipX(false);
	}
	else 
	{
		moveAnimation->FlipX(true);
		attackAnimation->FlipX(true);
	}

	if (attackAnimation->IsPlaying()) 
	{
		elapsedTime += _dt;
		if (elapsedTime >= attackSpeed) 
		{
			batDamageEvent.shotDirection = toPlayer;
			batDamageEvent.hitInfo.hitComponent = (PrimitiveComponent*) player->rootComponent;
			player->TakeDamage(damage, batDamageEvent, nullptr, this);

			elapsedTime -= attackSpeed;
		}
	}
	else 
	{
		if (!moveAnimation->IsPlaying())
		{
			moveAnimation->Trigger(true);
			moveAnimation->isVisible = true;

			attackAnimation->Trigger(false);
			attackAnimation->isVisible = false;

			elapsedTime = 0.50f;
		}

		if (distanceToPlayerSquared <= attackRange * attackRange)
		{
			// 어택 애니메이션 돌리고 
			moveAnimation->Trigger(false);
			moveAnimation->isVisible = false;

			attackAnimation->Trigger(true);
			attackAnimation->isVisible = true;

			SetVelocity(Math::Vector2::Zero);
		}
		else
		{
			추적_타이머 -= _dt;
			if (추적_타이머 <= 0.f)
			{
				AddVelocity(toPlayer * speed * 0.9f);
				추적_타이머 = 0.f;
			}
			else
			{
				AddVelocity(toPlayer * speed * 0.005f);
			}
		}
	}
}

void Bat::ReflectionIn()
{
	//auto reflectionResource = ResourceManager::LoadResource<ReflectionResource>();
}
