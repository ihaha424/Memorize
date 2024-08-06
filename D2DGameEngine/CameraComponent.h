#pragma once
#include "SceneComponent.h"

class Pawn;

class CameraComponent : public SceneComponent
{
public:
    CameraComponent();
    virtual ~CameraComponent();

    virtual void Update(const float& deltaTime);

    bool InCameraArea(Actor* actor);

private:
    static const Math::Vector2 screenSize;
    static const Math::Vector2 mapSize;
};

