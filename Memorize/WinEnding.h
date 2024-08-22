#pragma once
#include "D2DGameEngine/Level.h"


class WinEnding : public Level
{
    class WinEndingPanel* mainPanel;

public:
    WinEnding(class World* _world, const std::wstring& _name);
    virtual ~WinEnding();

    virtual void Enter() override;
    virtual void Exit();
    virtual void Clear();

};
