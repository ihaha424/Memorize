#pragma once

class GameObject;
class Level
{
public:
    Level(const std::wstring& name);
    virtual ~Level();

public:
    template<typename T>
    T* CreateGameObject()
    {
        bool b_IsBase = std::is_base_of<GameObject, T>::value;
        T* p_GameObject = new T();
        vGameObjectList.push_back(p_GameObject);
        return p_GameObject;
    }

public:
    virtual void    Enter() = 0;
    virtual void    Exit() = 0;
    virtual void    Clear() = 0;
    virtual void    Update(const float& deltaTime);
    virtual void    Render(ID2D1HwndRenderTarget* pRenderTarge);
    virtual void    LateUpdate(const float& deltaTime);
    void            PhysicalUpdate();

protected:
    std::vector<GameObject*>    vGameObjectList;
    //ColliderManager             colliderManager;

private:
    std::wstring LevelName;
};