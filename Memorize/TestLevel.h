#pragma once
#include "../D2DGameEngine/Level.h"
#include "D2DGameEngine/DotTween.h"

class TestLevel : public Level
{
   
public:
    TestLevel(class World* _world, const std::wstring& _name);
    virtual ~TestLevel();


    class BossHPPanel* bossHpBar = nullptr;
    class DisfellPanel* disfellPanel = nullptr;
    class PlayerMainUIPanel* playerMainUI = nullptr;
    virtual void Enter() override;
    virtual void Exit() {};
    virtual void Clear();

    float timeSclaeValue{ 1.f };
    DotTween<float>* timeScale;
};

