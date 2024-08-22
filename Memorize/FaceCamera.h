#pragma once

#include "common.h"

#include "D2DGameEngine/CameraComponent.h"
#include "../D2DGameEngine/DotTween.h"

class FaceCamera : public CameraComponent
{
public:
    FaceCamera(Actor* _owner);
    virtual ~FaceCamera();

    virtual void BeginPlay() override;
    virtual void Update(float _dt) override;
    void SetActor(class Actor* _Player, class Actor* _Boss);

private:
    class Actor* Player;
    class Actor* Boss;

   

    float       cameraZoom;
    DotTween<float>* cameraZoomDotTween;

    float       cameraX;
    DotTween<float>* cameraXTween;

    float delayTime = 2.f;
};

