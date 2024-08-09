#pragma once

#include "../D2DGameEngine/FSMComponent.h"
#include "PlayerState.h"
#include "PlayerStates.h"

class PlayerFSMComponent :
    public FSMComponent
{
    enum PlayerSkillType {Projectile, Range, Buff, Special};
    enum PlayerSkillProperty { Projectile, Range, Buff, Special };
protected:

public:
    PlayerFSMComponent(Actor* _owner) : FSMComponent(_owner) { SetTickProperties(TICK_UPDATE); };
    virtual ~PlayerFSMComponent() {};


    void BeginPlay() override;

    void InputKey(int _key);
};

