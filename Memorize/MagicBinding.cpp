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

	chainingTime = 1.f / 12.f * 13.f;

	floorChaining = CreateComponent<AnimationBitmapComponent>();
	floorChaining->SetSprite(L"TestResource/Boss/MagicBinding/FloorChaining.png");
	floorChaining->SliceSpriteSheet(150.f, 80.f, 0, 0, 0, 0);
	floorChaining->SetFrameDurations({ 1.f / 12.f });
	floorChaining->FrameResize(12);
	floorChaining->Trigger(true);

	floorChained = CreateComponent<AnimationBitmapComponent>();
	floorChained->isVisible = false;
	floorChained->SetSprite(L"TestResource/Boss/MagicBinding/FloorChained.png");
	floorChaining->SliceSpriteSheet(150.f, 80.f, 0, 0, 0, 0);
	floorChaining->SetFrameDurations({ 1.f / 12.f });
	floorChaining->Trigger(false);
	floorChaining->SetLoop(true);

	bodyChain = CreateComponent<AnimationBitmapComponent>();
	bodyChain->SetSprite(L"TestResource/Boss/MagicBinding/BodyChain.png");
	bodyChain->SliceSpriteSheet(550.f, 550.f, 0, 0, 0, 0);
	bodyChain->SetFrameDurations({ 1.f / 12.f });
	bodyChain->FrameSplice(1, bodyChain->GetFrameSize());
	bodyChain->Trigger(true);


	// Release Animation
	floorRelease = CreateComponent<AnimationBitmapComponent>();
	floorRelease->isVisible = false;
	floorRelease->SetSprite(L"TestResource/Boss/MagicBinding/FloorChaining.png");
	floorRelease->SliceSpriteSheet(150.f, 80.f, 0, 0, 0, 0);
	floorRelease->SetFrameDurations({ releasingTime / 12.f });
	floorRelease->FrameResize(12);
	// TODO: Reverse
	floorRelease->Trigger(false);

	bodyRelease = CreateComponent<AnimationBitmapComponent>();
	bodyRelease->isVisible = false;
	bodyRelease->SetSprite(L"TestResource/Boss/MagicBinding/BodyChain.png");
	bodyRelease->SliceSpriteSheet(550.f, 550.f, 0, 0, 0, 0);
	bodyRelease->SetFrameDurations({ releasingTime / 14.f });
	bodyRelease->FrameSplice(1, 15);
	// TODO: Reverse
	bodyRelease->Trigger(false);


	bm->AddChild(floorChaining);
	bm->AddChild(floorChained);
	bm->AddChild(bodyChain);
	bm->AddChild(floorRelease);
	bm->AddChild(bodyRelease);
}

void MagicBinding::BeginPlay()
{
	Super::BeginPlay();

	player = GetWorld()->FindActorByType<Player>();
	// TODO: bind player
	Math::Vector2 playerLocation = player->GetLocation();
	SetLocation(playerLocation.x, playerLocation.y);
}

void MagicBinding::FixedUpdate(float _fixedRate)
{
}

void MagicBinding::Update(float _dt)
{
	if (release) {
		releasingTime -= _dt;

		if (releasingTime <= 0.f)
		{
			Destroy();
			return;
		}

		if (chained)
		{
			floorChained->isVisible = false;
			floorChained->Trigger(false);

			floorRelease->isVisible = true;
			floorRelease->Trigger(true);

			bodyRelease->isVisible = true;
			bodyRelease->Trigger(true);
		}
		else
		{
			// TODO: reverse play floorChaining and bodyChain
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
				floorChaining->Trigger(false);
				floorChained->isVisible = true;
				floorChained->Trigger(true);

				chained = true;
			}
		}

		skillDuration -= _dt;
		if (skillDuration <= 0.f)
		{
			DisfellAction();
		}
	}
}

void MagicBinding::DisfellAction()
{
	release = true;
	// TODO: release player
	//player.
}

void MagicBinding::ReflectionIn()
{
}

void MagicBinding::ReflectionOut()
{
}
