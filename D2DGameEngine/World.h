#pragma once

class Level;

class World
{
public:
    World();
    virtual ~World();

public:
    template<typename T>
    T* CreateLevel(const std::wstring& LevelName)
    {
        bool IsBase = std::is_base_of<Level, T>::value;
        assert(IsBase == true);
        T* pLevel = new T(LevelName);
        vLevelList.insert({ LevelName, pLevel });
        return pLevel;
    }

    void ChangeScene();
    void SetNextScene(std::wstring nextLevel);

    virtual void FixedUpdate(float _fixedRate);
    virtual void PreUpdate(float _dt);
    virtual void Update(float _dt);
    virtual void PostUpdate(float _dt);
    virtual void Render(class D2DRenderer* _renderer);


protected:
    std::map<std::wstring, Level*>  vLevelList;
    Level* NextLevel = nullptr;
    Level* CurLevel = nullptr;
};