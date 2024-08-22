#include "LoadingLevel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Canvas.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/D2DRenderer.h"
#include "config.h"

LoadingLevel::LoadingLevel(World* _world, const std::wstring& _name)
	: Level(_world, _name)
{}

LoadingLevel::~LoadingLevel()
{
}

void LoadingLevel::Enter()
{
	__super::Enter();
	// 가라 로딩
	{
		if (!LoadImages(L"../TestResource")) {}
			//실패
	}
}

void LoadingLevel::Exit()
{
}

void LoadingLevel::Clear()
{
	for (auto& pGameObject : actorList)
	{
		delete pGameObject;
	}
	actorList.clear();
}

void LoadingLevel::FixedUpdate(float _fixedRate)
{
}

void LoadingLevel::PreUpdate(float _dt)
{
}

void LoadingLevel::Update(float _dt)
{
	GetWorld()->SetNextScene(L"MainLevel");
} 

void LoadingLevel::PostUpdate(float _dt)
{
}

void LoadingLevel::Render(D2DRenderer* _renderer)
{
	Math::Matrix mat = Math::Matrix::CreateTranslation(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f);
	_renderer->PushTransform(mat);
	for (auto resource : loadingResource)
	{
		_renderer->DrawSprite(resource->GetResource());
	}
}

bool LoadingLevel::LoadImages(const wchar_t* szPath)
{
    {
		std::filesystem::path path = szPath;
		auto paths = std::filesystem::directory_iterator(path);

        // Tour all files in that path
        for (const auto& entry : paths) {
			if (entry.is_directory())
			{
				std::wstring tempPath = szPath + std::wstring(L"/") + entry.path().filename().wstring();
				if (!LoadImages(tempPath.c_str()))
					return false;
			}
            else if (entry.path().extension() == L".png")
			{
                // Extract File Name
                std::wstring filename = szPath + std::wstring(L"/") + entry.path().filename().wstring();
				filename.erase(0,3);

                // Create Bitmap Object and Add to _images Map
				std::shared_ptr<SpriteResource> temp = ResourceManager::LoadResource<SpriteResource>(filename.c_str());
				if (temp == nullptr)
					return false;
				loadingResource.push_back(temp);
            }
        }

		return true;
    }
}

bool LoadingLevel::LoadSound()
{
	//UI
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_UI_Button01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_UI_Button02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_UI_Button03.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_UI_Button04.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_UI_Click.wav", 0);

	//Boss
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Boss_Pattern_Activated.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Boss_Laser.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Boss_Meteor01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Boss_Meteor02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Boss_Pattern02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Boss_Projectile01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Boss_Projectile02.wav", 0);

	//Playerr
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_BasicAttack01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_PlayerAttack01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_PlayerCasting02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_PlayerDeath.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_PlayerDispel01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_PlayerDispel03.wav", 0);

	//PlayerSkill
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_AggressiveWaves.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_ChasingWaterBall01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_ChasingWaterBall02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_DarkSphere01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_DarkSphere02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_ElementalExplosion01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_ElementalExplosion02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Fireball01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Fireball02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Flash.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Heal.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_LightStream.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_ManaOverload.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Memorize.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Meteor02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_MPMaxIncrease.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_MPrecovery.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_PrismReflection01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_PrismReflection02.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_Purification.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_TighteningCircle.wav", 0);


	// BackGournd
	ResourceManager::LoadResource(L"TestResource/Sound/BG/Sound_ClearBGM.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/BG/Sound_DeathBGM.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/BG/Sound_GameBGM01.wav", 1);
	ResourceManager::LoadResource(L"TestResource/Sound/BG/Sound_GameBGM02.wav", 1);
	ResourceManager::LoadResource(L"TestResource/Sound/BG/Sound_GameBGM03.wav", 1);
	ResourceManager::LoadResource(L"TestResource/Sound/BG/Sound_MainBGM.wav", 1);








	return true;
}
