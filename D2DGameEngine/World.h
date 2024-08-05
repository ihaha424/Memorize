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

    virtual void    Update(const float& deltaTime);
    virtual void    Render(ID2D1HwndRenderTarget* pRenderTarge);
    virtual void    LateUpdate(const float& deltaTime);
    void            PhysicalUpdate();

protected:
    std::map<std::wstring, Level*>  vLevelList;
    Level* NextLevel = nullptr;
    Level* CurLevel = nullptr;
};