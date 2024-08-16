#include "TestLevel.h"
#include "TestWall.h"
#include "Player.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/SpriteResource.h"
#include "../D2DGameEngine/BitmapComponent.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "GPlayerController.h"
#include "BossAIController.h"
#include "Boss.h"
#include "ElementsPanel.h"
#include "ManaDepletedPanel.h"
#include "BossHPPanel.h"
#include "Signal.h"
#include "DisfellPanel.h"
//#include "Bat.h"

TestLevel::TestLevel(class World* _world, const std::wstring& _name) : Level(_world, _name)
{
	
}

TestLevel::~TestLevel()
{
}

#include "BossGrowCircle.h"

void TestLevel::Enter()
{
	{
		Actor* actor = CreateActor<Actor>();
		actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
		BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
		bm->SetSprite(L"TestResource/BackGround_Map.png");
		actor->rootComponent = bm;
		actor->renderLayer = 0;
		//bm->SetTranslation(960, -540);
	}

	{
		Player* player = CreateActor<Player>();
		
		GPlayerController* pc = CreateActor<GPlayerController>();
		player->SetController(pc);
		pc->SetPlayer(player);

		disfellPanel = CreateActor<DisfellPanel>();
		pc->OnBeginDisfell->Connect([&](int index, int command) {disfellPanel->SetCommandImage(index, command); });
		pc->OnDoingDisfell->Connect([&](int index) {disfellPanel->HideCommandImage(index); });

		CreateActor<BossGrowCircle>();

		{
			ElementsPanel* elementsPanel = GetWorld()->GetCanvas()->CreatePannel<ElementsPanel>(L"Elements");
			elementsPanel->SetPlayerController(pc);
		}

		Boss* boss = CreateActor<Boss>();
		BossAIController* bc = CreateActor<BossAIController>();
		boss->SetController(bc);
		bc->SetBoss(boss);

		bossHpBar = GetWorld()->GetCanvas()->CreatePannel<BossHPPanel>(L"BossHPBar");
		boss->OnHPChanged->Connect([&](int hp) { bossHpBar->SetValue(hp); });
	}

	{
		ManaDepletedPanel* manaPanel = GetWorld()->GetCanvas()->CreatePannel< ManaDepletedPanel>(L"ManaDepleted");
	}


	{


	}

	{
		/*TestWall* testWall = CreateActor<TestWall>();
		testWall->SetLocation(960, -300);*/
		/*Bat* bat = CreateActor<Bat>();
		bat->SetLocation(-100, 100);*/
	}
}
