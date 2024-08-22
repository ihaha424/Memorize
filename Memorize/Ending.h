#pragma once
#include "D2DGameEngine/Level.h"


class Ending : public Level
{
    class EndingUIPanel* mainPanel;

public:
    Ending(class World* _world, const std::wstring& _name);
    virtual ~Ending();

    virtual void Enter() override;
    virtual void Exit();
    virtual void Clear();

};
