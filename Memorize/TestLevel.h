#pragma once
#include "../D2DGameEngine/Level.h"

class TestLevel : public Level
{
   
public:
    TestLevel(const std::wstring& name);
    virtual ~TestLevel();

    virtual void Enter() {};
    virtual void Exit() {};
    virtual void Clear() {};
};

