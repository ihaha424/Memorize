#pragma once

class Actor;

class Level
{
public:
    Level(const std::wstring& name);
    virtual ~Level();

public:
    template<typename T>
    T* CreateActor()
    {
        static_assert(std::is_base_of<Actor, T>::value, "T must inherit from Actor");
        T* newActor = new T();
        actorList.push_back(newActor);
        return newActor;
    }

public:
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Clear() = 0;
    
    virtual void FixedUpdate(float _fixedRate);
    virtual void PreUpdate(float _dt);
    virtual void Update(float _dt);
    virtual void PostUpdate(float _dt);
    virtual void Render(class D2DRenderer* _renderer);

protected:
    std::vector<Actor*> actorList;

private:
    std::wstring LevelName;
};