#pragma once
#include "../D2DGameEngine/CameraComponent.h"

class GCameraComponent : public CameraComponent
{
public:
    GCameraComponent(Actor* _owner);
    virtual ~GCameraComponent();

    virtual void BeginPlay() override;
    virtual void PostUpdate(float _dt) override;

private:
    class Boss* boss;
    float initialDistance;
    Math::Vector2 prevBossPos;
    float moveSecond{1.f};
    float moveSpeed{ 0.f};
    float cameraScale;
    bool  isMove{ false };
};

