#pragma once

#include "common.h"

#include "D2DGameEngine/CameraComponent.h"

class GCameraComponent : public CameraComponent
{
	float 피격Timer{ 0.f };
	float 피격opacity{ 0.f };
	class BitmapComponent* 피격;
	float 마나부족opacity{ 0.f };
	class BitmapComponent* 마나부족;
public:
    GCameraComponent(Actor* _owner);
    virtual ~GCameraComponent();

		void Trigger피격();
		void Trigger마나부족();

    virtual void BeginPlay() override;
		virtual void Update(float _dt) override;
    virtual void PostUpdate(float _dt) override;
		virtual void Render(class D2DRenderer* _renderer) override;

private:
    class Boss* boss;
    float initialDistance;
    Math::Vector2 prevBossPos;
    float moveSecond{1.f};
    float moveSpeed{ 0.f};
		float cameraScale{ 1.f };
    bool  isMove{ false };
};

