#include "Tutorials.h"
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
#include "TutorialPlayer.h"

#include "D2DGameEngine/Timer.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/KeyBoard.h"

#include "FadeInFadeOut.h"

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
	
	Arena* arena = CreateActor<Arena>();

	player = CreateActor<TutorialPlayer>();

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

	bossHpBar = GetWorld()->GetCanvas()->CreatePannel<BossHPPanel>(L"BossHPBar");
	boss->OnHPChanged->Connect([&](float hp) { bossHpBar->SetValue(hp); });
	
	// �÷��̾� ī�޶� Enermy ������
	player->GetComponent<GCameraComponent>()->SetCharacter(boss);


	ManaDepletedPanel* manaPanel = GetWorld()->GetCanvas()->CreatePannel< ManaDepletedPanel>(L"ManaDepleted");

	SetupTutorialGuide();
}


void Tutorials::Update(float _dt)
{
	Super::Update(_dt);

	tutorialGuide.Update(_dt);

	if (!statusRegen)
	{
		player->stat.hp = 50;
		player->stat.mp = 50;
	}
	else
	{
		player->stat.hp = player->stat.maxHp;
		player->stat.mp = player->stat.maxMp;
	}
}

void Tutorials::SetupTutorialGuide()
{
	{ // ���� ������� �Գ�
		auto text1ToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 3.f;
			};

		BitmapComponent* text1 = player->CreateComponent<BitmapComponent>();
		text1->SetSprite(L"TestResource/Tutorial/tutorial_text_1.png");
		text1->isVisible = false;
		player->orb->AddChild(text1);
		text1->SetTranslation(150, -90);

		auto text1ToggleAction = [this, text1]() {
			text1->isVisible = true;
			};

		auto text1ExitAction = [this, text1]() {
			text1->isVisible = false;
			};

		tutorialGuide.AddToggleAction(text1ToggleCondition, text1ToggleAction, text1ExitAction);
	}
	
	{	// �����Ѱž�?
		auto text2ToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text2 = player->CreateComponent<BitmapComponent>();
		text2->SetSprite(L"TestResource/Tutorial/tutorial_text_2.png");
		text2->isVisible = false;
		player->orb->AddChild(text2);
		text2->SetTranslation(150, -100);

		auto text2ToggleAction = [this, text2]() {
			text2->isVisible = true;
			};

		auto text2ExitAction = [this, text2]() {
			text2->isVisible = false;
			};

		tutorialGuide.AddToggleAction(text2ToggleCondition, text2ToggleAction, text2ExitAction);
	}
	
	{	// �� �׷���
		auto text3ToggleCondition = [this, count = 0](float dt) mutable {

			if (Mouse::clickStaus[1] == KeyDown)
				count++;
			return count > 4;
			};

		BitmapComponent* text3 = player->CreateComponent<BitmapComponent>();
		text3->SetSprite(L"TestResource/Tutorial/tutorial_text_3.png");
		text3->isVisible = false;
		player->orb->AddChild(text3);
		text3->SetTranslation(150, -100);

		auto text3ToggleAction = [this, text3]() {
			text3->isVisible = true;
			};

		auto text3ExitAction = [this, text3]() {
			text3->isVisible = false;
			};

		tutorialGuide.AddToggleAction(text3ToggleCondition, text3ToggleAction, text3ExitAction);
	}
	
	{	// �����߳�����
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_4.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(160, -100);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// QWER��
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_5.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(200, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// QQ�� ����
		auto textToggleCondition = [this](float dt) mutable {
			return playerController->curSkillInfo.id == SkilID{{ ST_PROJECTILE, SE_FIRE}}.id;
		};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_6.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(200, -100);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ���ϴ� �������� 
		auto textToggleCondition = [this](float dt) mutable {
			return Mouse::clickStaus[0] == KeyDown;
		};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_7.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(180, -100);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ���߾�!
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
		};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_8.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(150, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ������ �ͼ�������
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_9.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(180, -130);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// �������ͽ�
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_10.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(150, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ������ ü�� �������� ������
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_11.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(200, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			statusRegen = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ������ ������ ��ų�� ���� ����...
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_12.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(200, -120);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ������ �޸������
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_13.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(150, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// TabŰ�� ���� ��ų�� �����غ�
		auto textToggleCondition = [this](float dt) mutable {
			return playerController->CheckMemorize();
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_14.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(220, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ����. �ٽ� TabŰ�� 
		static bool condition{ false };

		struct Temp {
			void turnOn() {
				condition = true;
			};
		};
		
		Temp tmp;

		auto textToggleCondition = [this, &tmp](float dt) mutable {

			InputAction::GetInstance()->AddCommand(&tmp, DIK_TAB, &Temp::turnOn, InputState::KeyDown);

			return condition;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_15.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(200, -100);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, &tmp, text]() {
			text->isVisible = false;
			InputAction::GetInstance()->DeleteCommand(&tmp, DIK_TAB, InputState::KeyDown);
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// Space Bar�� ����
		static bool condition{ false };

		struct Temp {
			void turnOn() {
				condition = true;
			};
		};

		Temp tmp;

		auto textToggleCondition = [this, &tmp](float dt) mutable {

			InputAction::GetInstance()->AddCommand(&tmp, DIK_SPACE, &Temp::turnOn, InputState::KeyDown);

			return condition;
		};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_16.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(220, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, &tmp, text]() {
			text->isVisible = false;
			InputAction::GetInstance()->DeleteCommand(&tmp, DIK_SPACE, InputState::KeyDown);
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ���� ���� �������� ������
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_17.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(220, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// �� ������
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_18.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(200, -100);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// �������� �༮��
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_19.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(200, -100);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ��Ŭ������ �����غ�
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_20.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(240, -100);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// �غ�� ������ ��Ǹ���
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_21.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(200, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ������ ����
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 2.f;
			};

		BitmapComponent* text = player->CreateComponent<BitmapComponent>();
		text->SetSprite(L"TestResource/Tutorial/tutorial_text_22.png");
		text->isVisible = false;
		player->orb->AddChild(text);
		text->SetTranslation(100, -90);

		auto textToggleAction = [this, text]() {
			text->isVisible = true;
			};

		auto textExitAction = [this, text]() {
			text->isVisible = false;
			};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}

	{	// ��
		auto textToggleCondition = [elapsedTime = 0.f](float dt) mutable {
			elapsedTime += dt;
			return elapsedTime >= 3.f;
			};

		auto textToggleAction = [this]() {
			FadeInFadeOut* playerDie = GetWorld()->GetCanvas()->CreatePannel<FadeInFadeOut>(L"TestLevelFadeOut");
			playerDie->SetFaidInOut(false);
			playerDie->SetFinishFunction([this]()->void {
				GetWorld()->SetNextScene(L"PracticeLevel");
				});
			playerDie->alphaTween->SetDuration(3.f);
			playerDie->alphaTween->SetEasingEffect(EasingEffect::InQuart);
		};

		auto textExitAction = []() {};

		tutorialGuide.AddToggleAction(textToggleCondition, textToggleAction, textExitAction);
	}
}
