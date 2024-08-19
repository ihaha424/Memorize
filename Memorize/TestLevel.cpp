#include "TestLevel.h"
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
#include "ElementsPanel.h"
#include "DisfellPanel.h"
#include "BossHPPanel.h"
#include "ManaDepletedPanel.h"
//#include "Bat.h"
#include "BossRazer.h"
#include "BossChasingBomb.h"
#include "RazerCircle.h"

#include "PlayerMainUIPanel.h"

TestLevel::TestLevel(class World* _world, const std::wstring& _name) : Level(_world, _name)
{

}

TestLevel::~TestLevel()
{
}

#include "BossThroughProjectileMagicCircle.h"
#include "../D2DGameEngine/Timer.h"
void TestLevel::Enter()
{
	
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
		pc->OnDoingDisfell->Connect([&](int index) {disfellPanel->HideCommandImage(index); });

		//Timer::SetTimeScale(0.5);
		BossThroughProjectileMagicCircle* temp = CreateActor<BossThroughProjectileMagicCircle>();

		
		{
			ElementsPanel* elementsPanel = GetWorld()->GetCanvas()->CreatePannel<ElementsPanel>(L"Elements");
			elementsPanel->SetPlayerController(pc);
		}

		{
			playerMainUI = GetWorld()->GetCanvas()->CreatePannel<PlayerMainUIPanel>(L"PlayerMainUI");
			player->OnHPChanged->Connect([&](float hpValue) { playerMainUI->SetHPValue(hpValue); });
			player->OnMPChanged->Connect([&](float mpValue) { playerMainUI->SetMPValue(mpValue); });
		}
	}
	

	{
		Boss* boss = CreateActor<Boss>();
		BossAIController* bc = CreateActor<BossAIController>();
		boss->SetController(bc);
		bc->SetBoss(boss);

		bossHpBar = GetWorld()->GetCanvas()->CreatePannel<BossHPPanel>(L"BossHPBar");
		boss->OnHPChanged->Connect([&](float hp) { bossHpBar->SetValue(hp); });
	}



	{
		ManaDepletedPanel* manaPanel = GetWorld()->GetCanvas()->CreatePannel< ManaDepletedPanel>(L"ManaDepleted");
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

	/*{
		BossRazer* razer = CreateActor<BossRazer>();
		razer->renderLayer = 2;
	{
		/*BossRazer* razer = CreateActor<BossRazer>();
		razer->renderLayer = 2;*/

		/*RazerCircle* razerCircle = CreateActor<RazerCircle>();
		razerCircle->renderLayer = 3;*/

		/*ChasingBomb* chasingBomb = CreateActor<ChasingBomb>();
		chasingBomb->SetLocation(0.f, -400.f);
		chasingBomb->renderLayer = 2;
	}*/
}
