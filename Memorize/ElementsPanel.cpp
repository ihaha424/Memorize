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
	SetSkillList();
}

void ElementsPanel::SetQWER(std::vector<std::vector<int>>& elementCommands)
{
	HideAllCommands();
	for (int y = 0; y < elementCommands.size(); y++)
	{
		for (int x = 0; x < elementCommands[y].size(); x++)
		{
			switch (elementCommands[y][x])
			{
			case 0:
				commands[y][x]->SetSprite(Qbm);
				break;
			case 1:
				commands[y][x]->SetSprite(Wbm);
				break;
			case 2:
				commands[y][x]->SetSprite(Ebm);
				break;
			case 3:
				commands[y][x]->SetSprite(Rbm);
				break;
			}
			commands[y][x]->Activate();
		}
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

void ElementsPanel::SelectFireSkill()
{
	HideAllCommands();


	//Fireball
	commands[0][0]->Activate();
	commands[0][1]->Activate();
	commands[0][0]->SetSprite(L"TestResource/UI/Q.png");
	commands[0][1]->SetSprite(L"TestResource/UI/Q.png");

	//Meteor
	commands[1][0]->Activate();
	commands[1][1]->Activate();
	commands[1][2]->Activate();
	commands[1][3]->Activate();
	commands[1][0]->SetSprite(L"TestResource/UI/W.png");
	commands[1][1]->SetSprite(L"TestResource/UI/Q.png");
	commands[1][2]->SetSprite(L"TestResource/UI/Q.png");
	commands[1][3]->SetSprite(L"TestResource/UI/E.png");

	//Enchant
	commands[2][0]->Activate();
	commands[2][1]->Activate();
	commands[2][2]->Activate();
	commands[2][0]->SetSprite(L"TestResource/UI/E.png");
	commands[2][1]->SetSprite(L"TestResource/UI/R.png");
	commands[2][2]->SetSprite(L"TestResource/UI/Q.png");

	//ElementalExplosion
	commands[3][0]->Activate();
	commands[3][1]->Activate();
	commands[3][2]->Activate();
	commands[3][3]->Activate();
	commands[3][4]->Activate();
	commands[3][5]->Activate();
	commands[3][0]->SetSprite(L"TestResource/UI/R.png");
	commands[3][1]->SetSprite(L"TestResource/UI/E.png");
	commands[3][2]->SetSprite(L"TestResource/UI/W.png");
	commands[3][3]->SetSprite(L"TestResource/UI/Q.png");
	commands[3][4]->SetSprite(L"TestResource/UI/Q.png");
	commands[3][5]->SetSprite(L"TestResource/UI/W.png");

}

void ElementsPanel::SelectWaterSkill()
{
	HideAllCommands();
	q->SetSprite(L"TestResource/UI/Skill/Fireball.png");
	w->SetSprite(L"TestResource/UI/Skill/Meteor.png");
	e->SetSprite(L"TestResource/UI/Skill/Enchant.png");
	r->SetSprite(L"TestResource/UI/Skill/ElementalExplosion.png");

	//ChasingWaterBall
	commands[0][0]->Activate();
	commands[0][1]->Activate();
	commands[0][2]->Activate();
	commands[0][0]->SetSprite(L"TestResource/UI/Q.png");
	commands[0][1]->SetSprite(L"TestResource/UI/W.png");
	commands[0][2]->SetSprite(L"TestResource/UI/W.png");

	//Aggressive Waves
	commands[1][0]->Activate();
	commands[1][1]->Activate();
	commands[1][2]->Activate();
	commands[1][3]->Activate();
	commands[1][0]->SetSprite(L"TestResource/UI/W.png");
	commands[1][1]->SetSprite(L"TestResource/UI/W.png");
	commands[1][2]->SetSprite(L"TestResource/UI/E.png");
	commands[1][3]->SetSprite(L"TestResource/UI/W.png");

	//Mana Overload
	commands[2][0]->Activate();
	commands[2][1]->Activate();
	commands[2][2]->Activate();
	commands[2][3]->Activate();
	commands[2][0]->SetSprite(L"TestResource/UI/E.png");
	commands[2][1]->SetSprite(L"TestResource/UI/R.png");
	commands[2][2]->SetSprite(L"TestResource/UI/Q.png");
	commands[2][3]->SetSprite(L"TestResource/UI/E.png");

	//Heal
	commands[3][0]->Activate();
	commands[3][1]->Activate();
	commands[3][0]->SetSprite(L"TestResource/UI/R.png");
	commands[3][1]->SetSprite(L"TestResource/UI/E.png");

}

void ElementsPanel::SelectLightSkill()
{
	HideAllCommands();
	q->SetSprite(L"TestResource/UI/Skill/Fireball.png");
	w->SetSprite(L"TestResource/UI/Skill/Meteor.png");
	e->SetSprite(L"TestResource/UI/Skill/Enchant.png");
	r->SetSprite(L"TestResource/UI/Skill/ElementalExplosion.png");

	//Prism Reflection
	commands[0][0]->Activate();
	commands[0][1]->Activate();
	commands[0][0]->SetSprite(L"TestResource/UI/Q.png");
	commands[0][1]->SetSprite(L"TestResource/UI/E.png");

	//Light Stream
	commands[1][0]->Activate();
	commands[1][1]->Activate();
	commands[1][2]->Activate();
	commands[1][3]->Activate();
	commands[1][0]->SetSprite(L"TestResource/UI/W.png");
	commands[1][1]->SetSprite(L"TestResource/UI/Q.png");
	commands[1][2]->SetSprite(L"TestResource/UI/E.png");
	commands[1][3]->SetSprite(L"TestResource/UI/E.png");

	//MP Max Increase
	commands[2][0]->Activate();
	commands[2][1]->Activate();
	commands[2][2]->Activate();
	commands[2][0]->SetSprite(L"TestResource/UI/E.png");
	commands[2][1]->SetSprite(L"TestResource/UI/W.png");
	commands[2][2]->SetSprite(L"TestResource/UI/W.png");

	//Purification
	commands[3][0]->Activate();
	commands[3][1]->Activate();
	commands[3][2]->Activate();
	commands[3][3]->Activate();
	commands[3][4]->Activate();
	commands[3][5]->Activate();
	commands[3][0]->SetSprite(L"TestResource/UI/R.png");
	commands[3][1]->SetSprite(L"TestResource/UI/E.png");
	commands[3][2]->SetSprite(L"TestResource/UI/E.png");
	commands[3][3]->SetSprite(L"TestResource/UI/Q.png");
	commands[3][4]->SetSprite(L"TestResource/UI/W.png");
	commands[3][5]->SetSprite(L"TestResource/UI/W.png");
}

void ElementsPanel::SelectDarkSkill()
{
	HideAllCommands();
	q->SetSprite(L"TestResource/UI/Skill/Fireball.png");
	w->SetSprite(L"TestResource/UI/Skill/Meteor.png");
	e->SetSprite(L"TestResource/UI/Skill/Enchant.png");
	r->SetSprite(L"TestResource/UI/Skill/ElementalExplosion.png");

	//Dark Sphere
	commands[0][0]->Activate();
	commands[0][1]->Activate();
	commands[0][2]->Activate();
	commands[0][3]->Activate();
	commands[0][0]->SetSprite(L"TestResource/UI/Q.png");
	commands[0][1]->SetSprite(L"TestResource/UI/E.png");
	commands[0][2]->SetSprite(L"TestResource/UI/E.png");
	commands[0][3]->SetSprite(L"TestResource/UI/E.png");

	//Tightening Circle
	commands[1][0]->Activate();
	commands[1][1]->Activate();
	commands[1][0]->SetSprite(L"TestResource/UI/W.png");
	commands[1][1]->SetSprite(L"TestResource/UI/R.png");

	//MP recovery
	commands[2][0]->Activate();
	commands[2][1]->Activate();
	commands[2][2]->Activate();
	commands[2][0]->SetSprite(L"TestResource/UI/E.png");
	commands[2][1]->SetSprite(L"TestResource/UI/R.png");
	commands[2][2]->SetSprite(L"TestResource/UI/R.png");

	//Elemental Master
	commands[3][0]->Activate();
	commands[3][1]->Activate();
	commands[3][2]->Activate();
	commands[3][3]->Activate();
	commands[3][4]->Activate();
	commands[3][5]->Activate();
	commands[3][0]->SetSprite(L"TestResource/UI/R.png");
	commands[3][1]->SetSprite(L"TestResource/UI/E.png");
	commands[3][2]->SetSprite(L"TestResource/UI/W.png");
	commands[3][3]->SetSprite(L"TestResource/UI/Q.png");
	commands[3][4]->SetSprite(L"TestResource/UI/W.png");
	commands[3][5]->SetSprite(L"TestResource/UI/E.png");
}
