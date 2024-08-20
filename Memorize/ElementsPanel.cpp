#include "ElementsPanel.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/UIText.h"
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

	Qbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button01.png")->GetResource();
	Wbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button02.png")->GetResource();
	Ebm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button03.png")->GetResource();
	Rbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button04.png")->GetResource();
	Qbm_off = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button05.png")->GetResource();
	Wbm_off = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button06.png")->GetResource();
	Ebm_off = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button07.png")->GetResource();
	Rbm_off = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button08.png")->GetResource();

	q = CreateUI<UIImage>(L"Q");
	q->SetSprite(Qbm);
	q->SetPosition(0, -180);

	w = CreateUI<UIImage>(L"W");
	w->SetSprite(Wbm);
	w->SetPosition(0, -60);

	e = CreateUI<UIImage>(L"E");
	e->SetSprite(Ebm);
	e->SetPosition(0, 60);

	r = CreateUI<UIImage>(L"R");
	r->SetSprite(Rbm);
	r->SetPosition(0, 180);

	
	//이미지 배열
	commands.resize(4);
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			UIImage* image = CreateUI<UIImage>(L"command"+ y + x);
			commands[y].push_back(image);
			image->SetPosition(120 + x * 120, -180 + 120 * y);
			image->Inactivate();
		}
	}

	for (int i = 0; i < 4; i++)
	{
		infoTexts.push_back(CreateUI<UIText>(L"infoText_" + i));
		infoTexts[i]->SetColor(D2D_Color::Black);
		infoTexts[i]->SetFontSize(15);
		infoTexts[i]->SetSize(500, 100);
		infoTexts[i]->SetPosition(250, -100 + 120 * i);
		infoTexts[i]->SetWeight(FontWeight::Bold);
		infoTexts[i]->Inactivate();
	}

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
			q->SetSprite(Qbm);
			w->SetSprite(Wbm);
			e->SetSprite(Ebm);
			r->SetSprite(Rbm);
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
		infoTexts[curSkillType]->Activate();
		infoTexts[curSkillType]->SetText(playerController->FindCurSkiil()->GetInfoText());

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
			q->SetSprite(Qbm);
			w->SetSprite(Wbm);
			e->SetSprite(Ebm);
			r->SetSprite(Rbm);
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
		infoTexts[y]->Inactivate();
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
		q->SetSprite(L"TestResource/Icon/Icon_Fireball.png");
		w->SetSprite(L"TestResource/Icon/Icon_Meteor.png");
		e->SetSprite(L"TestResource/Icon/Icon_Enchant.png");
		r->SetSprite(L"TestResource/Icon/Icon_ElementalExplosion.png");
		SetQWER(fireCommands);
	}
	else if (nowElement == SE_WATER)
	{
		q->SetSprite(L"TestResource/Icon/Icon_ChasingWaterBall.png");
		w->SetSprite(L"TestResource/Icon/Icon_AggressiveWaves.png");
		e->SetSprite(L"TestResource/Icon/Icon_ManaOverload.png");
		r->SetSprite(L"TestResource/Icon/Icon_Heal.png");
		SetQWER(waterCommands);
	}
	else if (nowElement == SE_LIGHT)
	{
		q->SetSprite(L"TestResource/Icon/Icon_PrismReflection.png");
		w->SetSprite(L"TestResource/Icon/Icon_LightStream.png");
		e->SetSprite(L"TestResource/Icon/Icon_MPMaxIncrease.png");
		r->SetSprite(L"TestResource/Icon/Icon_Purification.png");
		SetQWER(lightCommands);
	}
	else if (nowElement == SE_DARKNESS)
	{
		q->SetSprite(L"TestResource/Icon/Icon_DarkSphere.png");
		w->SetSprite(L"TestResource/Icon/Icon_TighteningCircle.png");
		e->SetSprite(L"TestResource/Icon/Icon_MPrecovery.png");
		r->SetSprite(L"TestResource/Icon/Icon_ElementalMaster.png");
		SetQWER(darkCommands);
	}
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
