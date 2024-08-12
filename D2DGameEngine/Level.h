#pragma once

#include "Actor.h"

class Level
{
	using ActorMap = std::unordered_multimap<std::type_index, Actor*>;
	
protected:
	using ComponentRenderSequence = std::multimap<float, Actor*>;
	ComponentRenderSequence actorRenderSequence;
	

public:
    Level(class World* _world, const std::wstring& _name);
    virtual ~Level();

public:
    template<ActorType T>
    T* CreateActor()
    {
        T* newActor = new T(world);
        actorList.push_back(newActor);
				actorRenderSequence.insert({ 0.f, newActor });
        return newActor;
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

protected:
    std::vector<Actor*> actorList;

private:
    std::wstring LevelName;
    class World* world = nullptr;
};