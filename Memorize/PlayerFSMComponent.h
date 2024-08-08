#pragma once

#include "../D2DGameEngine/FSMComponent.h"
#include "PlayerState.h"
#include "PlayerStates.h"

class PlayerFSMComponent :
    public FSMComponent
{
public:
    PlayerFSMComponent() {};
    virtual ~PlayerFSMComponent() {};


    void BeginPlay() override;

    void InputKey(int _key);
};

