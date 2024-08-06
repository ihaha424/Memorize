#pragma once

#include "Singleton.h"
#include "Debug.h"
#include "Resource.h"
#include "SoundResource.h"


class ID2D1HwndRenderTarget;

class ResourceManager : public Singleton<ResourceManager>
{
	using ImageTag = std::wstring; // is equal to the path
	using ResourceData = std::shared_ptr<IResource>;
private:
	static std::unordered_map<ImageTag, ResourceData> resourceStorage;
	
	//ResourcePath
	static wchar_t resourcePath[255];
	static ID2D1HwndRenderTarget* renderTarget;


	static void InitialPath();


	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) noexcept = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) noexcept = delete;


public:
	ResourceManager() {};
	~ResourceManager() {};

	static void Initialize(ID2D1HwndRenderTarget* _render);
	static ID2D1Bitmap* CreateID2D1Bitmap(std::wstring _key);

	template<typename T>
	static std::shared_ptr<T> LoadResource(const ImageTag& _tag)
	{
		std::shared_ptr<T> sprite = SearchResource<T>(_tag);
		if (sprite != nullptr)
			return sprite;

		std::wstring strFilePath = resourcePath;
		strFilePath += _tag;
		sprite = std::make_shared<T>();
		sprite->LoadFile(_tag);
		sprite->SetKey(_tag);
		resourceStorage.insert({ _tag, sprite });
		return sprite;
	}

	static std::shared_ptr<SoundResource> LoadResource(const ImageTag& _tag, bool _loopCheck)
	{
		std::shared_ptr<SoundResource> sprite = SearchResource<SoundResource>(_tag);
		if (sprite != nullptr)
			return sprite;

		std::wstring strFilePath = resourcePath;
		strFilePath += _tag;
		sprite = std::make_shared<SoundResource>();
		sprite->LoadFileLoop(_tag, _loopCheck);
		sprite->SetKey(_tag);
		resourceStorage.insert({ _tag, sprite });
		return sprite;
	}

private:
	template<typename T>
	static std::shared_ptr<T> SearchResource(const ImageTag& _tag)
	{
		auto iter = resourceStorage.find(_tag);
		if (iter == resourceStorage.end())
		{
			return nullptr;
		}
		if (iter->second->GetType() == T::TYPE_ID)
			return std::static_pointer_cast<T>(iter->second);
		else
			LOG_ERROR(-1, "ResourceType is Diffrent.");
		return nullptr;
	}

};

