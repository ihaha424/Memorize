#pragma once

#include "Actor.h"

class Level
{
public:
    Level(class World* _world, const std::wstring& _name);
    virtual ~Level();

public:
    template<ActorType T>
    T* CreateActor()
    {
        T* newActor = new T();
        newActor->SetWorld(world);
        actorList.push_back(newActor);
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
    virtual void Render(class D2DRenderer* _renderer);

protected:
    std::vector<Actor*> actorList;

private:
    std::wstring LevelName;
    class World* world = nullptr;
};