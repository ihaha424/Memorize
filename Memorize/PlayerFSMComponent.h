#pragma once

#include "../D2DGameEngine/FSMComponent.h"
#include "PlayerState.h"
#include "PlayerStates.h"

class PlayerFSMComponent :
    public FSMComponent
{
public:
    enum InputEvent { Fire, Water, Light, Dark, Attack, Move };
public:
    PlayerFSMComponent() { SetTickProperties(TICK_UPDATE); };
    virtual ~PlayerFSMComponent() {};


    void BeginPlay() override;

    void InputKey(int _key);
};

