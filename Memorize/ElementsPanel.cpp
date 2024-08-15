#include "ElementsPanel.h"
#include "D2DGameEngine/UIImage.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "GPlayerController.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/SpriteResource.h"

ElementsPanel::ElementsPanel(World* _world) : UIPanel(_world)
{
	fireCommands = { {0,0}, {1, 0, 0, 2}, {2, 3, 1},{3, 2, 2, 0, 0, 2} };
	waterCommands = { {0, 1, 1}, {1, 1, 2, 1}, {2, 3, 1, 3},{3, 2} };
	lightCommands = { {0, 2}, {1, 0, 2, 2}, {2, 1, 1},{3, 2, 2, 1, 2, 2} };
	darkCommands = { {0, 3, 3, 3}, {1, 3}, {2, 3, 3},{3, 2, 1, 0, 1, 2} };

	SetPosition(70, 240);

	Qbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Q.png")->GetResource();
	Wbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/W.png")->GetResource();
	Ebm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/E.png")->GetResource();
	Rbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/R.png")->GetResource();

	q = CreateUI<UIImage>(L"Q");
	q->SetSprite(Qbm);
	q->SetPosition(0, -90);

	w = CreateUI<UIImage>(L"W");
	w->SetSprite(Wbm);
	w->SetPosition(0, -30);

	e = CreateUI<UIImage>(L"E");
	e->SetSprite(Ebm);
	e->SetPosition(0, 30);

	r = CreateUI<UIImage>(L"R");
	r->SetSprite(Rbm);
	r->SetPosition(0, 90);

	
	//이미지 배열
	commands.resize(4);
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			UIImage* image = CreateUI<UIImage>(L"command"+ y + x);
			commands[y].push_back(image);
			image->SetPosition(60 + x * 60, -90 + 60 * y);
			image->Inactivate();
		}
	}

}

ElementsPanel::~ElementsPanel()
{
}


void ElementsPanel::Update(float _dt)
{
	ESkillType curSkillType = playerController->GetCurSkillInfo().type;
	ESkillElement curSkillElement = playerController->GetCurSkillInfo().element;

	if (playerController->isPlayerAfterCasting())
	{
		HideAllCommands();
		q->SetSprite(Qbm);
		w->SetSprite(Wbm);
		e->SetSprite(Ebm);
		r->SetSprite(Rbm);
		return;
	}

	//스킬 타입까지 정해진 경우
	if (curSkillType != ST_END)
	{
		HideAllCommands();
		SetQWER(CheckSkillType(), playerController->GetCurSkillInfo().type);

		//커맨드를 입력중
		int curSkillInputCommand = playerController->GetPlayerCastingIndex();
		for (int i = 0; i < curSkillInputCommand; i++)
		{
			commands[curSkillType][i]->Inactivate();
		}
		
	}
	//스킬 원소만 정해진 경우
	else if (curSkillElement != SE_END)
	{
		SetSkillList();
	}
	else
	{
		HideAllCommands();
		q->SetSprite(Qbm);
		w->SetSprite(Wbm);
		e->SetSprite(Ebm);
		r->SetSprite(Rbm);
	}
}

void ElementsPanel::SetQWER(std::vector<std::vector<int>>& elementCommands)
{
	HideAllCommands();
	for (int y = 0; y < elementCommands.size(); y++)
	{
		SetQWER(elementCommands, y);
	}
}

void ElementsPanel::SetQWER(std::vector<std::vector<int>>& elementCommands, int type)
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
		q->SetSprite(L"TestResource/UI/Skill/Fireball.png");
		w->SetSprite(L"TestResource/UI/Skill/Meteor.png");
		e->SetSprite(L"TestResource/UI/Skill/Enchant.png");
		r->SetSprite(L"TestResource/UI/Skill/ElementalExplosion.png");
		SetQWER(fireCommands);
	}
	else if (nowElement == SE_WATER)
	{
		q->SetSprite(L"TestResource/UI/Skill/Fireball.png");
		w->SetSprite(L"TestResource/UI/Skill/Meteor.png");
		e->SetSprite(L"TestResource/UI/Skill/Enchant.png");
		r->SetSprite(L"TestResource/UI/Skill/ElementalExplosion.png");
		SetQWER(waterCommands);
	}
	else if (nowElement == SE_LIGHT)
	{
		q->SetSprite(L"TestResource/UI/Skill/Fireball.png");
		w->SetSprite(L"TestResource/UI/Skill/Meteor.png");
		e->SetSprite(L"TestResource/UI/Skill/Enchant.png");
		r->SetSprite(L"TestResource/UI/Skill/ElementalExplosion.png");
		SetQWER(lightCommands);
	}
	else if (nowElement == SE_DARKNESS)
	{
		q->SetSprite(L"TestResource/UI/Skill/Fireball.png");
		w->SetSprite(L"TestResource/UI/Skill/Meteor.png");
		e->SetSprite(L"TestResource/UI/Skill/Enchant.png");
		r->SetSprite(L"TestResource/UI/Skill/ElementalExplosion.png");
		SetQWER(darkCommands);
	}
}

std::vector<std::vector<int>>& ElementsPanel::CheckSkillType()
{
	std::vector<std::vector<int>>& elementCommands = fireCommands;
	switch (playerController->GetCurSkillInfo().element)
	{
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
