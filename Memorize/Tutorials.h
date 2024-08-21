#pragma once
#include "../D2DGameEngine/Level.h"

class Tutorials : public Level
{

public:
    Tutorials(class World* _world, const std::wstring& _name);
    virtual ~Tutorials();


    class BossHPPanel* bossHpBar = nullptr;
    class DisfellPanel* disfellPanel = nullptr;
    class PlayerMainUIPanel* playerMainUI = nullptr;
    virtual void Enter() override;
    virtual void Exit() {};
    virtual void Clear();
};

