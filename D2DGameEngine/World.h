#pragma once

#include "Level.h"


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
        dynamic_cast<Level*>(pLevel)->SetWorld(this);
        levelList.insert({ LevelName, pLevel });
        return pLevel;
    }

    void ChangeScene();
    void SetNextScene(std::wstring nextLevel);

    void AddUICanvas(class Canvas* canvas);
    void RemoveUICanvas(class Canvas* canvas);

    /**
     * @brief 메인 카메라 컴포넌트를 설정합니다. 
     * @param component 메인 카메라 컴포넌트
     */
    void SetMainCamera(class CameraComponent* _camera) { mainCamera = _camera; };
    class CameraComponent* GetMainCamera() { return mainCamera; };

    virtual void BeginPlay();
    virtual void FixedUpdate(float _fixedRate);
    virtual void PreUpdate(float _dt);
    virtual void Update(float _dt);
    virtual void PostUpdate(float _dt);
    virtual void Render(class D2DRenderer* _renderer);


protected:
    std::map<std::wstring, Level*>  levelList;
    Level* NextLevel = nullptr;
    Level* CurLevel = nullptr;

    class CameraComponent* mainCamera = nullptr;

    std::vector<class Canvas*> activeUICanvasList;
};