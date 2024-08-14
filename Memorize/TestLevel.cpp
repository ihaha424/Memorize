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

TestLevel::TestLevel(class World* _world, const std::wstring& _name) : Level(_world, _name)
{
	
}

TestLevel::~TestLevel()
{
}
#include "Pattern06.h"
void TestLevel::Enter()
{
	{
		Actor* actor = CreateActor<Actor>();
		actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
		BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
		bm->SetSprite(L"TestResource/BackGround_Map.png");
		actor->rootComponent = bm;
		//bm->SetTranslation(960, -540);
	}

	{
		Player* player = CreateActor<Player>();
		
		GPlayerController* pc = CreateActor<GPlayerController>();
		player->SetController(pc);
		pc->SetPlayer(player);

		CreateActor<Pattern06>();
	}

	{
		GetWorld()->GetCanvas()->CreatePannel<ElementsPanel>(L"Elements");
	}

	{
		//Boss* boss = CreateActor<Boss>();
		//BossAIController* bc = CreateActor<BossAIController>();
		//boss->SetController(bc);
	}

	{
		/*TestWall* testWall = CreateActor<TestWall>();
		testWall->SetLocation(960, -300);*/
	}
}
