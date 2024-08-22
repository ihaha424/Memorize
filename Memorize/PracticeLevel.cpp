#include "PracticeLevel.h"

#include "Arena.h"
#include "Player.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/SpriteResource.h"
#include "TutorialTextBitmap.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "GPlayerController.h"
#include "BossAIController.h"

#include "Boss.h"
#include "BossHPPanel.h"
#include "Scarecrow.h"

#include "ElementsPanel.h"
#include "DisfellPanel.h"
#include "ManaDepletedPanel.h"

#include "MainLevelUIPanel.h"

#include "PlayerMainUIPanel.h"
#include "CursorUIPanel.h"

#include "GCameraComponent.h"

#include "Cursor.h"
#include "Player.h"

#include "D2DGameEngine/Timer.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/KeyBoard.h"

#include "ScarecrowMagicCircle.h"

#include "FadeInFadeOut.h"

Practice::Practice(World* _world, const std::wstring& _name) : Level(_world, _name)
{
}

Practice::~Practice()
{
}

void Practice::Enter()
{
	__super::Enter();

	GetWorld()->GetCanvas()->CreatePannel<CursorUIPanel>(L"Cursor");

	Arena* arena = CreateActor<Arena>();

	player = CreateActor<Player>();
	player->renderLayer = 2;

	player->SetLocation(-200.f, 0.f);

	playerController = CreateActor<GPlayerController>();
	player->SetController(playerController);
	playerController->SetPlayer(player);

	disfellPanel = GetWorld()->GetCanvas()->CreatePannel<DisfellPanel>(L"DisfellCommands");
	playerController->OnBeginDisfell->Connect([&](int index, int command) {disfellPanel->SetCommandImage(index, command); });
	playerController->OnDoingDisfell->Connect([&](int index, int command) {disfellPanel->CommandImageOff(index, command); });


	elementsPanel = GetWorld()->GetCanvas()->CreatePannel<ElementsPanel>(L"Elements");
	elementsPanel->SetPlayerController(playerController);


	playerMainUI = GetWorld()->GetCanvas()->CreatePannel<PlayerMainUIPanel>(L"PlayerMainUI");
	player->OnHPInfoChanged->Connect([&](float hp, float maxHp) { playerMainUI->SetHPInfo(hp, maxHp); });
	player->OnMPInfoChanged->Connect([&](float mp, float maxMp) { playerMainUI->SetMPInfo(mp, maxMp); });
	playerController->OnMemorize->Connect([&]() {playerMainUI->SetMemorizeOn(); });
	playerController->OffMemorize->Connect([&]() {playerMainUI->SetMemorizeOff(); });
	playerController->OnFlash->Connect([&]() {playerMainUI->SetFlashOn(); });
	playerController->OffFlash->Connect([&]() {playerMainUI->SetFlashOff(); });


	Scarecrow* boss = CreateActor<Scarecrow>();
	boss->SetLocation(500.f, 0);
	boss->renderLayer = 2;

	bossHpBar = GetWorld()->GetCanvas()->CreatePannel<BossHPPanel>(L"BossHPBar");
	boss->OnHPChanged->Connect([&](float hp) { bossHpBar->SetValue(hp); });

	// 플레이어 카메라에 Enermy 붙히기
	player->GetComponent<GCameraComponent>()->SetCharacter(boss);


	ManaDepletedPanel* manaPanel = GetWorld()->GetCanvas()->CreatePannel< ManaDepletedPanel>(L"ManaDepleted");

}

void Practice::Clear()
{
	actorRenderSequence.clear();
	actorTypeMap.clear();
	for (auto& pGameObject : actorList)
	{
		delete pGameObject;
	}
	actorList.clear();
}

void Practice::Update(float _dt)
{
	Super::Update(_dt);

	skillTimer -= _dt;
	if (skillTimer <= 0.f)
	{
		auto circle = CreateActor<ScarecrowMagicCircle>();
		circle->renderLayer = 1;
		skillTimer = 15.f;
	}

	player->stat.hp = player->stat.maxHp;
	player->stat.mp = player->stat.maxMp;
}
