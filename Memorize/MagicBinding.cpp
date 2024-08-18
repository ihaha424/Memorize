#include "MagicBinding.h"

#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/PolygonComponent.h"

#include "Player.h"

MagicBinding::MagicBinding(World* _world) : BossSkillActor(_world)
{
	bm->isVisible = false;

	isDispel = true;
	isFragile = true;
	type = Range;
	dispelTime = 3.f;
	skillDuration = 3.f;

	chainingTime = 1.f;

	floorChaining = CreateComponent<AnimationBitmapComponent>();
	floorChaining->SetSprite(L"TestResource/Boss/MagicBinding/FloorChaining.png");
	floorChaining->SliceSpriteSheet(150.f, 80.f, 0, 0, 0, 0);
	floorChaining->SetFrameDurations({ chainingTime / 12.f });
	floorChaining->Trigger(true);

	floorChained = CreateComponent<AnimationBitmapComponent>();
	floorChained->isVisible = false;
	floorChained->SetSprite(L"TestResource/Boss/MagicBinding/FloorChained.png");
	floorChained->SliceSpriteSheet(150.f, 80.f, 0, 0, 0, 0);
	floorChained->SetFrameDurations({ 1.0f / 12.f });
	floorChained->FrameResize(32);
	floorChained->Trigger(false);
	floorChained->SetLoop(true);

	bodyChaining = CreateComponent<AnimationBitmapComponent>();
	bodyChaining->SetSprite(L"TestResource/Boss/MagicBinding/BodyChain.png");
	bodyChaining->SliceSpriteSheet(550.f, 550.f, 0, 0, 0, 0);
	bodyChaining->SetFrameDurations({ chainingTime / 13.f });
	bodyChaining->FrameSplice(1, 15);
	bodyChaining->Trigger(true);

	bodyChained = CreateComponent<AnimationBitmapComponent>();
	bodyChained->isVisible = false;
	bodyChained->SetSprite(L"TestResource/Boss/MagicBinding/BodyChain.png");
	bodyChained->SliceSpriteSheet(550.f, 550.f, 0, 0, 0, 0);
	bodyChained->SetFrameDurations({ chainingTime / 12.f });
	bodyChained->FrameSplice(16, 97);
	bodyChained->Trigger(false);
	bodyChained->SetLoop(true);

	bm->AddChild(floorChaining);
	bm->AddChild(floorChained);
	bm->AddChild(bodyChaining);
	bm->AddChild(bodyChained);

	floorChaining->SetScale(2.f, 2.f);
	floorChaining->SetTranslation(0.f, 100.f);
	floorChained->SetScale(2.f, 2.f);
	floorChained->SetTranslation(-6.f, 103.5f);

	bodyChaining->SetTranslation(0.f, 0.f);
	bodyChained->SetTranslation(0.f, 0.f);
}

void MagicBinding::BeginPlay()
{
	Super::BeginPlay();

	player = GetWorld()->FindActorByType<Player>();
	player->bondageFlag = true;
	Math::Vector2 playerLocation = player->GetLocation();
	SetLocation(playerLocation.x, playerLocation.y);
}

void MagicBinding::FixedUpdate(float _fixedRate)
{
	Super::FixedUpdate(_fixedRate);
}

void MagicBinding::Update(float _dt)
{
	Super::Update(_dt);

	if (release) {
		releasingTime -= _dt;

		if (releasingTime <= 0.f)
		{
			Destroy();
			return;
		}
	}
	else
	{
		if (!chained)
		{
			chainingTime -= _dt;
			if (chainingTime <= 0.f)
			{
				floorChaining->isVisible = false;
				floorChaining->Pause();
				floorChained->isVisible = true;
				floorChained->Trigger(true);

				bodyChaining->isVisible = false;
				bodyChaining->Pause();
				bodyChained->isVisible = true;
				bodyChained->Trigger(true);

				chained = true;
			}
		}

		skillDuration -= _dt;
		if (skillDuration <= 0.f)
		{
			DisfellAction();
			floorChained->isVisible = false;
			floorChained->Trigger(false);

			bodyChained->isVisible = false;
			bodyChained->Trigger(false);

			floorChaining->isVisible = true;
			floorChaining->SetReverse(true);
			floorChaining->Resume();

			bodyChaining->isVisible = true;
			bodyChaining->SetReverse(true);
			bodyChaining->Resume();
		}
	}
}

void MagicBinding::DisfellAction()
{
	release = true;
	player->bondageFlag = false;
}

void MagicBinding::ReflectionIn()
{
}

void MagicBinding::ReflectionOut()
{
}
