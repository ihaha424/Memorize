#include "Tutorials.h"
#include "Arena.h"
#include "Player.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/SpriteResource.h"
#include "../D2DGameEngine/BitmapComponent.h"
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

#include "Cursor.h"

Tutorials::Tutorials(class World* _world, const std::wstring& _name) : Level(_world, _name)
{

}

Tutorials::~Tutorials()
{
}

void Tutorials::Clear()
{
	actorRenderSequence.clear();
	actorTypeMap.clear();
	for (auto& pGameObject : actorList)
	{
		delete pGameObject;
	}
	actorList.clear();
}

//#include "BossProjectile.h"
//#include "../D2DGameEngine/Timer.h"
void Tutorials::Enter()
{
	__super::Enter();

	GetWorld()->GetCanvas()->CreatePannel<CursorUIPanel>(L"Cursor");
	{
		Arena* arena = CreateActor<Arena>();
		/*Actor* actor = CreateActor<Actor>();
		actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
		BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
		bm->SetSprite(L"TestResource/BackGround_Map.png");
		actor->rootComponent = bm;
		actor->renderLayer = 0;*/
		//bm->SetTranslation(960, -540);
	}

	{
		Player* player = CreateActor<Player>();

		GPlayerController* pc = CreateActor<GPlayerController>();
		player->SetController(pc);
		pc->SetPlayer(player);

		disfellPanel = GetWorld()->GetCanvas()->CreatePannel<DisfellPanel>(L"DisfellCommands");
		pc->OnBeginDisfell->Connect([&](int index, int command) {disfellPanel->SetCommandImage(index, command); });
		pc->OnDoingDisfell->Connect([&](int index, int command) {disfellPanel->CommandImageOff(index, command); });

		// 보스 투사체 붙히기
		/*BossProjectile* proj1 =CreateActor<BossProjectile>();*/

		{
			ElementsPanel* elementsPanel = GetWorld()->GetCanvas()->CreatePannel<ElementsPanel>(L"Elements");
			elementsPanel->SetPlayerController(pc);
		}

		{
			playerMainUI = GetWorld()->GetCanvas()->CreatePannel<PlayerMainUIPanel>(L"PlayerMainUI");
			player->OnHPInfoChanged->Connect([&](float hp, float maxHp) { playerMainUI->SetHPInfo(hp, maxHp); });
			player->OnMPInfoChanged->Connect([&](float mp, float maxMp) { playerMainUI->SetMPInfo(mp, maxMp); });
			pc->OnMemorize->Connect([&]() {playerMainUI->SetMemorizeOn(); });
			pc->OffMemorize->Connect([&]() {playerMainUI->SetMemorizeOff(); });
			pc->OnFlash->Connect([&]() {playerMainUI->SetFlashOn(); });
			pc->OffFlash->Connect([&]() {playerMainUI->SetFlashOff(); });
		}
	}


	{
		Scarecrow* boss = CreateActor<Scarecrow>();
		boss->SetLocation(500.f, 0);
		/*BossAIController* bc = CreateActor<BossAIController>();
		boss->SetController(bc);
		bc->SetBoss(boss);*/

		bossHpBar = GetWorld()->GetCanvas()->CreatePannel<BossHPPanel>(L"BossHPBar");
		boss->OnHPChanged->Connect([&](float hp) { bossHpBar->SetValue(hp); });
	}



	{
		ManaDepletedPanel* manaPanel = GetWorld()->GetCanvas()->CreatePannel< ManaDepletedPanel>(L"ManaDepleted");
	}
}
