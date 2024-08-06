#pragma once
#include "../D2DGameEngine/CameraComponent.h"

class GCameraComponent : public CameraComponent
{
public:
    GCameraComponent();
    virtual ~GCameraComponent();

    virtual void Update(float _dt) override;
};

