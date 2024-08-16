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
//#include "Bat.h"
#include "BossRazer.h"

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

		////CreateActor<BossGrowMagicCircle>();

		CreateActor<BossGrowCircle>();

	}



	{
		//Boss* boss = CreateActor<Boss>();
		//BossAIController* bc = CreateActor<BossAIController>();
		//boss->SetController(bc);
	}

	{
		/*TestWall* testWall = CreateActor<TestWall>();
		testWall->SetLocation(960, -300);*/
		/*Bat* bat = CreateActor<Bat>();
		bat->SetLocation(-100, 100);*/
	}

	// Map vertex check
	//{
	//	Actor* actor = CreateActor<Actor>();
	//	actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	//	BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
	//	bm->SetSprite(L"TestResource/Boss/Bat/Attack/Bat_Attack_0.png");
	//	actor->rootComponent = bm;
	//	actor->SetLocation(-3120, 100);
	//	actor->renderLayer = 3;
	//}
	//{
	//	Actor* actor = CreateActor<Actor>();
	//	actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	//	BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
	//	bm->SetSprite(L"TestResource/Boss/Bat/Attack/Bat_Attack_0.png");
	//	actor->rootComponent = bm;
	//	actor->SetLocation(-3120, 260);
	//	actor->renderLayer = 3;
	//}
	//{
	//	Actor* actor = CreateActor<Actor>();
	//	actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	//	BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
	//	bm->SetSprite(L"TestResource/Boss/Bat/Attack/Bat_Attack_0.png");
	//	actor->rootComponent = bm;
	//	actor->SetLocation(0, -1780);
	//	actor->renderLayer = 3;
	//}
	//{
	//	Actor* actor = CreateActor<Actor>();
	//	actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	//	BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
	//	bm->SetSprite(L"TestResource/Boss/Bat/Attack/Bat_Attack_0.png");
	//	actor->rootComponent = bm;
	//	actor->SetLocation(3120, 100);
	//	actor->renderLayer = 3;
	//}
	//{
	//	Actor* actor = CreateActor<Actor>();
	//	actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	//	BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
	//	bm->SetSprite(L"TestResource/Boss/Bat/Attack/Bat_Attack_0.png");
	//	actor->rootComponent = bm;
	//	actor->SetLocation(3120, 260);
	//	actor->renderLayer = 3;
	//}
	//{
	//	Actor* actor = CreateActor<Actor>();
	//	actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	//	BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
	//	bm->SetSprite(L"TestResource/Boss/Bat/Attack/Bat_Attack_0.png");
	//	actor->rootComponent = bm;
	//	actor->SetLocation(0, 2140);
	//	actor->renderLayer = 3;
	//}
	//{
	//	Actor* actor = CreateActor<Actor>();
	//	actor->SetTickProperties(TICK_UPDATE | TICK_RENDER);
	//	BitmapComponent* bm = actor->CreateComponent<BitmapComponent>();
	//	bm->SetSprite(L"TestResource/Boss/Bat/Attack/Bat_Attack_0.png");
	//	actor->rootComponent = bm;
	//	//actor->SetLocation(-3118, 0);
	//	actor->renderLayer = 3;
	//}

	{
		BossRazer* razer = CreateActor<BossRazer>();
		razer->renderLayer = 2;
	}
}
