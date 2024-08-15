#pragma once

#include "../D2DGameEngine/FSMComponent.h"
#include "PlayerState.h"
#include "PlayerStates.h"

class PlayerFSMComponent :
    public FSMComponent
{
protected:

public:
    PlayerFSMComponent(Actor* _owner);
    virtual ~PlayerFSMComponent() {};


    void BeginPlay() override;

    void InputKey(int _key);
};

