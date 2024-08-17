#include "DisfellPanel.h"
#include "GPlayerController.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/SpriteResource.h"
#include "D2DGameEngine/UIImage.h"


DisfellPanel::DisfellPanel(World* _world) : UIPanel(_world)
{
	Qbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/Q.png")->GetResource();
	Wbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/W.png")->GetResource();
	Ebm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/E.png")->GetResource();
	Rbm = ResourceManager::GetInstance().LoadResource<SpriteResource>(L"TestResource/UI/R.png")->GetResource();

	for (int i = 0; i < commandMaxCount; i++)
	{
		disfellCommands.push_back(CreateUI<UIImage>(L"DisfellCommand" + i));
		disfellCommands[i]->Inactivate();
		disfellCommands[i]->SetPosition(1920/2 -600 + 50 * i, 30);
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
