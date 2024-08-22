#include "ElementsPanel.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/UIText.h"
#include "D2DGameEngine/UIButton.h"
#include "GPlayerController.h"
#include "ElementalMasterComponent.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/SpriteResource.h"

ElementsPanel::ElementsPanel(World* _world) : UIPanel(_world)
{
	fireCommands = { {0,0}, {1, 0, 0, 2}, {2, 3, 1},{3, 2, 2, 0, 0, 2} };
	waterCommands = { {0, 1, 1}, {1, 1, 2, 1}, {2, 3, 1, 3},{3, 2} };
	lightCommands = { {0, 2}, {1, 0, 2, 2}, {2, 1, 1},{3, 2, 2, 1, 2, 2} };
	darkCommands = { {0, 3, 3, 3}, {1, 3}, {2, 3, 3},{3, 2, 1, 0, 1, 2} };

	SetPosition(70, 1080/2);

	
	Qicon = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/Icon/Icon_Q.png")->GetResource();
	Wicon = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/Icon/Icon_W.png")->GetResource();
	Eicon = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/Icon/Icon_E.png")->GetResource();
	Ricon = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/Icon/Icon_R.png")->GetResource();
	
	Qbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button01.png")->GetResource();
	Wbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button02.png")->GetResource();
	Ebm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button03.png")->GetResource();
	Rbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button04.png")->GetResource();
	Qbm_off = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button05.png")->GetResource();
	Wbm_off = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button06.png")->GetResource();
	Ebm_off = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button07.png")->GetResource();
	Rbm_off = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button08.png")->GetResource();

	ring = CreateUI<UIImage>(L"Ring");
	ring->SetSprite(L"TestResource/Icon/UI_ring.png");
	ring->SetPosition(-12, 0);

	q = CreateUI<UIImage>(L"Q");
	q->SetSprite(Qicon);
	q->SetPosition(-30, -120);

	w = CreateUI<UIImage>(L"W");
	w->SetSprite(Wicon);
	w->SetPosition(8, -47);

	e = CreateUI<UIImage>(L"E");
	e->SetSprite(Eicon);
	e->SetPosition(8, 47);

	r = CreateUI<UIImage>(L"R");
	r->SetSprite(Ricon);
	r->SetPosition(-30, 120);

	qBtn = CreateUI<UIButton>(L"QButton");
	qBtn->SetPosition(-30, -120);
	qBtn->SetSize(60, 60);
	qBtn->AddOnHoveredHandler([=]() {ShowSkillInfo(ST_PROJECTILE); });
	qBtn->AddOnUnHoveredHandler([=]() {HideSkillInfo(ST_PROJECTILE); });
	qBtn->SetZOrder(10);

	wBtn = CreateUI<UIButton>(L"WButton");
	wBtn->SetPosition(8, -47);
	wBtn->SetSize(60, 60);
	wBtn->AddOnHoveredHandler([=]() {ShowSkillInfo(ST_RANGE); });
	wBtn->AddOnUnHoveredHandler([=]() {HideSkillInfo(ST_RANGE); });
	wBtn->SetZOrder(10);

	eBtn = CreateUI<UIButton>(L"EButton");
	eBtn->SetPosition(8, 47);
	eBtn->SetSize(60, 60);
	eBtn->AddOnHoveredHandler([=]() {ShowSkillInfo(ST_BUFF); });
	eBtn->AddOnUnHoveredHandler([=]() {HideSkillInfo(ST_BUFF); });
	eBtn->SetZOrder(10);

	rBtn = CreateUI<UIButton>(L"RButton");
	rBtn->SetPosition(-30, 120);
	rBtn->SetSize(60, 60);
	rBtn->AddOnHoveredHandler([=]() {ShowSkillInfo(ST_SPECIAL); });
	rBtn->AddOnUnHoveredHandler([=]() {HideSkillInfo(ST_SPECIAL); });
	rBtn->SetZOrder(10);

	//이미지 배열
	commands.resize(4);
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			UIImage* image = CreateUI<UIImage>(L"command"+ y + x);
			commands[y].push_back(image);
			image->SetSize(50, 50);
			image->Inactivate();
		}
	}

	for (int i = 0; i < 6; i++)
		commands[0][i]->SetPosition(30 + i * 60, -120);
	for (int i = 0; i < 6; i++)
		commands[1][i]->SetPosition(20 + 52 + i * 60, -47);
	for (int i = 0; i < 6; i++)
		commands[2][i]->SetPosition(20 + 52 + i * 60, 47);
	for (int i = 0; i < 6; i++)
		commands[3][i]->SetPosition(30 + i * 60, 120);

	for (int i = 0; i < 4; i++)
	{
		infoTexts.push_back(CreateUI<UIText>(L"infoText_" + i));
		infoTexts[i]->SetColor(D2D_Color::White);
		infoTexts[i]->SetFontSize(20);
		infoTexts[i]->SetSize(360, 110);
		infoTexts[i]->SetWeight(FontWeight::Bold);
		infoTexts[i]->Inactivate();

		textBGs.push_back(CreateUI<UIImage>(L"infoTextBackground_" + i));
		textBGs[i]->SetSprite(L"TestResource/UI/UI_skillexplanationBG.png");
		textBGs[i]->Inactivate();
	}

	infoTexts[0]->SetPosition(-30 + 200 + 5, -120 - 100 + 10);
	infoTexts[1]->SetPosition(8 + 200 + 5, -47 - 100 + 10);
	infoTexts[2]->SetPosition(8 + 200 + 5, 47 - 100 + 10);
	infoTexts[3]->SetPosition(-30 + 200 + 5, 120 - 100 + 10);

	textBGs[0]->SetPosition(-30 + 200, -120 - 100);
	textBGs[1]->SetPosition(8 + 200, -47 - 100);
	textBGs[2]->SetPosition(8 + 200, 47 - 100);
	textBGs[3]->SetPosition(-30 + 200, 120 - 100);
}

ElementsPanel::~ElementsPanel()
{
}


void ElementsPanel::Update(float _dt)
{
	__super::Update(_dt);
	ESkillType curSkillType = playerController->GetCurSkillInfo().type;
	ESkillElement curSkillElement = playerController->GetCurSkillInfo().element;


	//다 눌렀으면 0.5초 후에 사라지게 함 
	if (ending)
	{
		elapsedTime += _dt;
		if (elapsedTime > 0.5f)
		{
			elapsedTime = 0.f;
			ending = false;
		}
	}

	if (playerController->isPlayerAfterCasting() || playerController->bElementalMaster)
	{
		if (!ending || playerController->bElementalMaster)
		{
			HideAllCommands();
			q->SetSprite(Qicon);
			w->SetSprite(Wicon);
			e->SetSprite(Eicon);
			r->SetSprite(Ricon);
		}
		return;
	}
	else if (playerController->GetPlayerState() == L"PlayerBlinking")
	{
		return;
	}

	//스킬 타입까지 정해진 경우
	if (curSkillElement != SE_END && curSkillType != ST_END)
	{
		ending = false;
		HideAllCommands();
		SetQWER(CheckSkillType(), playerController->GetCurSkillInfo().type);
		
		//커맨드를 입력중
		int curSkillInputCommand = playerController->GetPlayerCastingIndex();
		for (int i = 0; i < curSkillInputCommand; i++)
		{
			SetOff(CheckSkillType(), curSkillType, i);
			ending = true;
		}
		
	}
	//스킬 원소만 정해진 경우
	else if (curSkillElement != SE_END)
	{
		SetSkillList();
	}
	else
	{
		if(!ending)
		{
			HideAllCommands();
			q->SetSprite(Qicon);
			w->SetSprite(Wicon);
			e->SetSprite(Eicon);
			r->SetSprite(Ricon);
		}
	}
}

void ElementsPanel::SetQWER(std::vector<std::vector<int>> elementCommands)
{
	HideAllCommands();
	for (int y = 0; y < elementCommands.size(); y++)
	{
		SetQWER(elementCommands, y);
	}
}

void ElementsPanel::SetQWER(std::vector<std::vector<int>> elementCommands, int type)
{
	for (int x = 0; x < elementCommands[type].size(); x++)
	{
		switch (elementCommands[type][x])
		{
		case 0:
			commands[type][x]->SetSprite(Qbm);

			break;
		case 1:
			commands[type][x]->SetSprite(Wbm);
			break;
		case 2:
			commands[type][x]->SetSprite(Ebm);
			break;
		case 3:
			commands[type][x]->SetSprite(Rbm);
			break;
		}
		commands[type][x]->Activate();
	}
}

void ElementsPanel::SetOff(std::vector<std::vector<int>> elementCommands, int type, int index)
{
	switch (elementCommands[type][index])
	{
	case 0:
		commands[type][index]->SetSprite(Qbm_off);
		break;
	case 1:
		commands[type][index]->SetSprite(Wbm_off);
		break;
	case 2:
		commands[type][index]->SetSprite(Ebm_off);
		break;
	case 3:
		commands[type][index]->SetSprite(Rbm_off);
		break;
	}
	commands[type][index]->Activate();

}

void ElementsPanel::HideAllCommands()
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			commands[y][x]->Inactivate();
		}
	}
}

void ElementsPanel::SetSkillList()
{
	if (playerController == nullptr) return;

	ESkillElement nowElement = playerController->GetCurSkillInfo().element;

	if (nowElement == SE_FIRE)
	{
		if(playerController->FindSkiil(SE_FIRE, ST_PROJECTILE)->IsUnlocked())
			q->SetSprite(L"TestResource/Icon/Icon_Fireball.png");
		else
			q->SetSprite(L"TestResource/Icon/Icon_Fireball_off.png");

		if (playerController->FindSkiil(SE_FIRE, ST_RANGE)->IsUnlocked())
			w->SetSprite(L"TestResource/Icon/Icon_Meteor.png");
		else
			w->SetSprite(L"TestResource/Icon/Icon_Meteor_off.png");
		if (playerController->FindSkiil(SE_FIRE, ST_BUFF)->IsUnlocked())
			e->SetSprite(L"TestResource/Icon/Icon_Enchant.png");
		else
			e->SetSprite(L"TestResource/Icon/Icon_Enchant_off.png");
		if (playerController->FindSkiil(SE_FIRE, ST_SPECIAL)->IsUnlocked())
			r->SetSprite(L"TestResource/Icon/Icon_ElementalExplosion.png");
		else
			r->SetSprite(L"TestResource/Icon/Icon_ElementalExplosion_off.png");
		SetQWER(fireCommands);
	}
	else if (nowElement == SE_WATER)
	{
		if (playerController->FindSkiil(SE_WATER, ST_PROJECTILE)->IsUnlocked())
			q->SetSprite(L"TestResource/Icon/Icon_ChasingWaterBall.png");
		else
			q->SetSprite(L"TestResource/Icon/Icon_ChasingWaterBall_off.png");
		if (playerController->FindSkiil(SE_WATER, ST_RANGE)->IsUnlocked())
			w->SetSprite(L"TestResource/Icon/Icon_AggressiveWaves.png");
		else
			w->SetSprite(L"TestResource/Icon/Icon_AggressiveWaves_off.png");
		if (playerController->FindSkiil(SE_WATER, ST_BUFF)->IsUnlocked())
			e->SetSprite(L"TestResource/Icon/Icon_ManaOverload.png");
		else
			e->SetSprite(L"TestResource/Icon/Icon_ManaOverload_off.png");
		if (playerController->FindSkiil(SE_WATER, ST_SPECIAL)->IsUnlocked())
			r->SetSprite(L"TestResource/Icon/Icon_Heal.png");
		else
			r->SetSprite(L"TestResource/Icon/Icon_Heal_off.png");
		SetQWER(waterCommands);
	}
	else if (nowElement == SE_LIGHT)
	{
		if (playerController->FindSkiil(SE_LIGHT, ST_PROJECTILE)->IsUnlocked())
			q->SetSprite(L"TestResource/Icon/Icon_PrismReflection.png");
		else
			q->SetSprite(L"TestResource/Icon/Icon_PrismReflection_off.png");
		if (playerController->FindSkiil(SE_LIGHT, ST_RANGE)->IsUnlocked())
			w->SetSprite(L"TestResource/Icon/Icon_LightStream.png");
		else
			w->SetSprite(L"TestResource/Icon/Icon_LightStream_off.png");
		if (playerController->FindSkiil(SE_LIGHT, ST_BUFF)->IsUnlocked())
			e->SetSprite(L"TestResource/Icon/Icon_MPMaxIncrease.png");
		else
			e->SetSprite(L"TestResource/Icon/Icon_MPMaxIncrease_off.png");
		if (playerController->FindSkiil(SE_LIGHT, ST_SPECIAL)->IsUnlocked())
			r->SetSprite(L"TestResource/Icon/Icon_Purification.png");
		else
			r->SetSprite(L"TestResource/Icon/Icon_Purification_off.png");
		SetQWER(lightCommands);
	}
	else if (nowElement == SE_DARKNESS)
	{
		if (playerController->FindSkiil(SE_DARKNESS, ST_PROJECTILE)->IsUnlocked())
			q->SetSprite(L"TestResource/Icon/Icon_DarkSphere.png");
		else
			q->SetSprite(L"TestResource/Icon/Icon_DarkSphere_off.png");
		if (playerController->FindSkiil(SE_DARKNESS, ST_RANGE)->IsUnlocked())
			w->SetSprite(L"TestResource/Icon/Icon_TighteningCircle.png");
		else
			w->SetSprite(L"TestResource/Icon/Icon_TighteningCircle_off.png");
		if (playerController->FindSkiil(SE_DARKNESS, ST_BUFF)->IsUnlocked())
			e->SetSprite(L"TestResource/Icon/Icon_MPrecovery.png");
		else
			e->SetSprite(L"TestResource/Icon/Icon_MPrecovery_off.png");
		if (playerController->FindSkiil(SE_DARKNESS, ST_SPECIAL)->IsUnlocked())
			r->SetSprite(L"TestResource/Icon/Icon_ElementalMaster.png");
		else
			r->SetSprite(L"TestResource/Icon/Icon_ElementalMaster_off.png");
		SetQWER(darkCommands);
	}
}

void ElementsPanel::ShowSkillInfo(ESkillType curSkillType)
{
	ESkillElement element = playerController->GetCurSkillInfo().element;
	if (element != SE_FIRE && element != SE_WATER && element != SE_LIGHT && element != SE_DARKNESS)
		return;
	infoTexts[curSkillType]->Activate();
	textBGs[curSkillType]->Activate();
	infoTexts[curSkillType]->SetText(playerController->FindSkiil(element, curSkillType)->GetInfoText()
	+ L"\n" + playerController->FindSkiil(element, curSkillType)->GetInfoText2());
}

void ElementsPanel::HideSkillInfo(ESkillType curSkillType)
{
	infoTexts[curSkillType]->Inactivate();
	textBGs[curSkillType]->Inactivate();
}

std::vector<std::vector<int>> ElementsPanel::CheckSkillType()
{
	std::vector<std::vector<int>> elementCommands;
	switch (playerController->GetCurSkillInfo().element)
	{
	case SE_FIRE:
		elementCommands = fireCommands;
		break;
	case SE_WATER:
		elementCommands = waterCommands;
		break;
	case SE_LIGHT:
		elementCommands = lightCommands;
		break;
	case SE_DARKNESS:
		elementCommands = darkCommands;
		break;
	}
	return elementCommands;
}
