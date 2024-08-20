#pragma once

#include "common.h"

#include "D2DGameEngine/CameraComponent.h"

class GCameraComponent : public CameraComponent
{
	float �ǰ�Timer{ 0.f };
	float �ǰ�opacity{ 0.f };
	class BitmapComponent* �ǰ�;
	float ��������opacity{ 0.f };
	class BitmapComponent* ��������;
public:
    GCameraComponent(Actor* _owner);
    virtual ~GCameraComponent();

		void Trigger�ǰ�();
		void Trigger��������();

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

