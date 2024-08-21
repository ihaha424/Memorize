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
