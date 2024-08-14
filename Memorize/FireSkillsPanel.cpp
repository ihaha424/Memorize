#include "FireSkillsPanel.h"
#include "D2DGameEngine/UIImage.h"

FireSkillsPanel::FireSkillsPanel(World* _world) : UIPanel(_world)
{
	SetPosition(70, 240);

	q = CreateUI<UIImage>(L"Q_Fireball");
	q->SetSprite(L"TestResource/UI/Skill/Fireball.png");
	q->SetPosition(0, -90);

	w = CreateUI<UIImage>(L"W_Meteor");
	w->SetSprite(L"TestResource/UI/Skill/Meteor.png");
	w->SetPosition(0, -30);

	e = CreateUI<UIImage>(L"E_Enchant");
	e->SetSprite(L"TestResource/UI/Skill/Enchant.png");
	e->SetPosition(0, 30);

	r = CreateUI<UIImage>(L"R_ElementalExplosion");
	r->SetSprite(L"TestResource/UI/Skill/ElementalExplosion.png");
	r->SetPosition(0, 90);

	commands.resize(4);

	//Fireball
	UIImage* fireball_command0 = CreateUI<UIImage>(L"Fireball_command0");
	commands[0].push_back(fireball_command0);
	fireball_command0->SetSprite(L"TestResource/UI/Q.png");

	UIImage* fireball_command1 = CreateUI<UIImage>(L"Fireball_command1");
	commands[0].push_back(fireball_command1);
	fireball_command1->SetSprite(L"TestResource/UI/Q.png");

	//Meteor
	UIImage* meteor_command0 = CreateUI<UIImage>(L"Meteor_command0");
	commands[1].push_back(meteor_command0);
	meteor_command0->SetSprite(L"TestResource/UI/W.png");

	UIImage* meteor_command1 = CreateUI<UIImage>(L"Meteor_command1");
	commands[1].push_back(meteor_command1);
	meteor_command1->SetSprite(L"TestResource/UI/Q.png");

	UIImage* meteor_command2 = CreateUI<UIImage>(L"Meteor_command2");
	commands[1].push_back(meteor_command2);
	meteor_command2->SetSprite(L"TestResource/UI/Q.png");

	UIImage* meteor_command3 = CreateUI<UIImage>(L"Meteor_command3");
	commands[1].push_back(meteor_command3);
	meteor_command3->SetSprite(L"TestResource/UI/E.png");

	//Enchant
	UIImage* enchant_command0 = CreateUI<UIImage>(L"Enchant_command0");
	commands[2].push_back(enchant_command0);
	enchant_command0->SetSprite(L"TestResource/UI/E.png");

	UIImage* enchant_command1 = CreateUI<UIImage>(L"Enchant_command1");
	commands[2].push_back(enchant_command1);
	enchant_command1->SetSprite(L"TestResource/UI/R.png");

	UIImage* enchant_command2 = CreateUI<UIImage>(L"Enchant_command2");
	commands[2].push_back(enchant_command2);
	enchant_command2->SetSprite(L"TestResource/UI/Q.png");

	//ElementalExplosion
	UIImage* explosion_command0 = CreateUI<UIImage>(L"Explosion_command0");
	commands[3].push_back(explosion_command0);
	explosion_command0->SetSprite(L"TestResource/UI/R.png");

	UIImage* explosion_command1 = CreateUI<UIImage>(L"Explosion_command1");
	commands[3].push_back(explosion_command1);
	explosion_command1->SetSprite(L"TestResource/UI/E.png");

	UIImage* explosion_command2 = CreateUI<UIImage>(L"Explosion_command2");
	commands[3].push_back(explosion_command2);
	explosion_command2->SetSprite(L"TestResource/UI/W.png");

	UIImage* explosion_command3 = CreateUI<UIImage>(L"Explosion_command3");
	commands[3].push_back(explosion_command3);
	explosion_command3->SetSprite(L"TestResource/UI/Q.png");

	UIImage* explosion_command4 = CreateUI<UIImage>(L"Explosion_command4");
	commands[3].push_back(explosion_command4);
	explosion_command4->SetSprite(L"TestResource/UI/Q.png");

	UIImage* explosion_command5 = CreateUI<UIImage>(L"Explosion_command5");
	commands[3].push_back(explosion_command5);
	explosion_command5->SetSprite(L"TestResource/UI/W.png");


	//이미지 배열
	for (int y = 0; y < commands.size(); y++)
	{
		for (int x = 0; x < commands[y].size(); x++)
		{
			UIImage* image = commands[y][x];
			image->SetPosition(60 + x * 60, -90 + 60 * y);
		}
	}
}

FireSkillsPanel::~FireSkillsPanel()
{
}

void FireSkillsPanel::OnPressedQ()
{
	if (status == OS_INACTIVE) return;
}

void FireSkillsPanel::OnPressedW()
{
	if (status == OS_INACTIVE) return;
}

