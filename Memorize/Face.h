#pragma once
#include "D2DGameEngine/Level.h"


class Face : public Level
{
public:
    Face(class World* _world, const std::wstring& _name);
    virtual ~Face();

    virtual void Enter() override;
    virtual void Exit();
    virtual void Clear();

};
