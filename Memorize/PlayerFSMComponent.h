#pragma once

#include "../D2DGameEngine/FSMComponent.h"
#include "PlayerState.h"
#include "PlayerStates.h"

class PlayerFSMComponent :
    public FSMComponent
{
    enum PlayerSkillType {Projectile, Range, Buff, Special};
    enum PlayerSkillProperty { Fire, Water, Dark, Light };
protected:

public:
    PlayerFSMComponent(Actor* _owner) : FSMComponent(_owner) { SetTickProperties(TICK_UPDATE); };
    virtual ~PlayerFSMComponent() {};


    void BeginPlay() override;

    void InputKey(int _key);
};

