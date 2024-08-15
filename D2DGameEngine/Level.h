#pragma once

#include "Actor.h"

class Level
{
	// TODO: Find actor.
	using ActorTypeMap = std::unordered_multimap<std::type_index, Actor*>;
	
protected:
	using ComponentRenderSequence = std::multimap<std::pair<int, float>, Actor*>;
	ComponentRenderSequence actorRenderSequence;
	
	ActorTypeMap actorTypeMap;

public:
    Level(class World* _world, const std::wstring& _name);
    virtual ~Level();

public:
    template<ActorType T>
    T* CreateActor()
    {
        T* newActor = new T(world);
        actorList.push_back(newActor);
				actorTypeMap.insert({ std::type_index(typeid(T)), newActor });
				actorRenderSequence.insert({ { newActor->renderLayer,0.f }, newActor
	});
        return newActor;
    }

	/**
		* @brief 레벨에 존재하는 특정 타입의 액터 중 하나를 반환합니다. 
		* @tparam T 탐색할 액터의 타입
		* @return 액터를 찾지 못하면 nullptr, 아니면 액터의 포인터
	*/
	template<ActorType T>
	T* FindActorByType() 
	{
		auto it = actorTypeMap.find(std::type_index(typeid(T)));
		if (it == actorTypeMap.end()) return nullptr;
		return static_cast<T*>(it->second);
	}

	template<ActorType T>
	std::vector<T*> FindAllActorsByType()
	{
		std::vector<T*> res;
		auto range = actorTypeMap.equal_range(std::type_index(typeid(T)));
		for (auto it = range.first; it != range.second; ++it) {
			res.push_back(static_cast<T*>(it->second));
		}
		return res;
	}


public:

    void SetWorld(World* _world) { world = _world; }
    class World* GetWorld() { return world; }

    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Clear() = 0;
    
    virtual void BeginPlay();
    virtual void FixedUpdate(float _fixedRate);
    virtual void PreUpdate(float _dt);
    virtual void Update(float _dt);
    virtual void PostUpdate(float _dt);
	virtual void PrepareRender();
    virtual void Render(class D2DRenderer* _renderer);

    std::vector<Actor*> actorList;

private:
    std::wstring LevelName;
    class World* world = nullptr;
};