#include "DisfellPanel.h"
#include "GPlayerController.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/SpriteResource.h"
#include "D2DGameEngine/UIImage.h"


DisfellPanel::DisfellPanel(World* _world) : UIPanel(_world)
{
	Qbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button01.png")->GetResource();
	Wbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button02.png")->GetResource();
	Ebm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button03.png")->GetResource();
	Rbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Button04.png")->GetResource();

	for (int i = 0; i < commandMaxCount; i++)
	{
		disfellCommands.push_back(CreateUI<UIImage>(L"DisfellCommand" + i));
		disfellCommands[i]->Inactivate();
		disfellCommands[i]->SetPosition(1920/2 -300 + 100 * i, 100);
	}

}

DisfellPanel::~DisfellPanel()
{
}

void DisfellPanel::SetCommandImage(int index, int command)
{
	disfellCommands[index]->Activate();

	switch (command)
	{
	case SE_FIRE:
		disfellCommands[index]->SetSprite(Qbm);
		break;
	case SE_WATER:
		disfellCommands[index]->SetSprite(Wbm);
		break;
	case SE_LIGHT:
		disfellCommands[index]->SetSprite(Ebm);
		break;
	case SE_DARKNESS:
		disfellCommands[index]->SetSprite(Rbm);
		break;
	default:
		break;
	}

}

void DisfellPanel::HideCommandImage(int index)
{
	if (index < 0)
	{
		for (auto disfellCommands : disfellCommands)
			disfellCommands->Inactivate();
		return; 
	}
	disfellCommands[index]->Inactivate();
}
