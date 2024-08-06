#pragma once
#include "../D2DGameEngine/Level.h"

class TestLevel : public Level
{
   
public:
    TestLevel(class World* _world, const std::wstring& _name);
    virtual ~TestLevel();

    virtual void Enter();
    virtual void Exit() {};
    virtual void Clear() {};
};

