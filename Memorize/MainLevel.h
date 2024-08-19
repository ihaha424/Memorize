#pragma once
#include "D2DGameEngine/Level.h"

class MainLevel : public Level
{
    class MainLevelUIPanel* mainPanel;
public:
    MainLevel(class World* _world, const std::wstring& _name);
    virtual ~MainLevel();

    virtual void Enter();
    virtual void Exit();
    virtual void Clear();

};

