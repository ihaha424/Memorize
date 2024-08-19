#pragma once
#include "SceneComponent.h"

class Pawn;

class CameraComponent : public SceneComponent
{
public:
    CameraComponent(Actor* _owner);
    virtual ~CameraComponent();

    virtual void Update(float _dt) override;

    bool InCameraArea(Actor* actor);

    static const Math::Vector2 screenSize;
    static const Math::Vector2 mapSize;

};

