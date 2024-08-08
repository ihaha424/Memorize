#pragma once

#include "../D2DGameEngine/FSMComponent.h"
#include "PlayerStates.h"

class PlayerFSMComponent :
    public FSMComponent
{
public:
    PlayerFSMComponent(Actor* _owner) :FSMComponent(_owner){};
    virtual ~PlayerFSMComponent() {};


    void BeginPlay() override;

    void InputKey(int Key);
};

