#include "Face.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/Actor.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "FaceCamera.h"
#include "Arena.h"
#include "D2DGameEngine//SoundManager.h"

Face::Face(World* _world, const std::wstring& _name)
	: Level(_world, _name)
{}

Face::~Face()
{
}

void Face::Enter()
{
	SoundManager::PlayMusic(L"TestResource/Sound/BG/Sound_GameBGM.wav", 0);

	Arena* arena = CreateActor<Arena>();

	Actor* Player = CreateActor<Actor>();
	Player->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	AnimationBitmapComponent* PlayerBm = Player->CreateComponent<AnimationBitmapComponent>();
	PlayerBm->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	PlayerBm->SetSprite(L"TestResource/Player/PlayerMotions/PlayerIdle.png");
	PlayerBm->SliceSpriteSheet(137, 254, 0, 0, 0, 0);
	PlayerBm->SetFrameDurations({ 0.1f });
	PlayerBm->SetLoop(true);
	PlayerBm->Trigger(true);
	Player->rootComponent = PlayerBm;
	Player->renderLayer = 1;


	Actor* Boss = CreateActor<Actor>();
	Boss->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	AnimationBitmapComponent* BossBm = Boss->CreateComponent<AnimationBitmapComponent>();
	BossBm->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	BossBm->SetSprite(L"TestResource/Boss/BossMotions/Boss_Idle.png");
	BossBm->SliceSpriteSheet(187, 287, 0, 0, 0, 0);
	BossBm->SetFrameDurations({ 0.06f });
	BossBm->SetLoop(true);
	BossBm->Trigger(true);
	BossBm->SetTranslation(3000, 0);
	Boss->rootComponent = BossBm;
	Boss->renderLayer = 1;


	Actor* Camera = CreateActor<Actor>();
	Camera->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	FaceCamera* cm = Camera->CreateComponent<FaceCamera>();
	Camera->rootComponent = cm;
	cm->SetActor(Player, Boss);
	GetWorld()->SetMainCamera(cm);

	__super::Enter();
}

void Face::Exit()
{
	__super::Exit();
}

void Face::Clear()
{
	for (auto& pGameObject : actorList)
	{
		delete pGameObject;
	}
	actorList.clear();
}
