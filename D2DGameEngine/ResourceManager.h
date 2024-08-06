#pragma once

#include "Singleton.h"
#include "Debug.h"
#include "Resource.h"

class ID2D1HwndRenderTarget;

class ResourceManager : public Singleton<ResourceManager>
{
	using ImageTag = std::wstring; // is equal to the path
	using ResourceData = std::shared_ptr<IResource>;
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
	static T LoadResource(const ImageTag& _tag)
	{
		T sprite = SearchResource(_tag);
		if (sprite != nullptr)
		{
			if (sprite->GetType() == T::TYPE_ID)
				return sprite;
			else
				LOG_ERROR(-1, "ResourceType is Diffrent.");
		}

		std::wstring strFilePath = resourcePath;
		strFilePath += _tag;
		sprite = std::make_shared<T>();
		sprite->LoadFile(_tag);
		sprite->SetKey(_tag);
		resourceStorage.insert({ _tag, sprite });
		return sprite;
	}

private:
	template<typename T>
	static T SearchResource(const ImageTag& _tag)
	{
		auto iter = resourceStorage.find(_tag);
		if (iter == resourceStorage.end())
		{
			return nullptr;
		}
		return iter->second;
	}

};

