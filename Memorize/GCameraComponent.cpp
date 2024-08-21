#include "GCameraComponent.h"
#include "Boss.h"
#include "Scarecrow.h"

#include "../D2DGameEngine/World.h"

#include "D2DGameEngine/BitmapComponent.h"

#include "D2DGameEngine/D2DRenderer.h"


GCameraComponent::GCameraComponent(Actor* _owner) : CameraComponent(_owner)
{
	SetTickProperties(TICK_UPDATE | TICK_POST_UPDATE | TICK_RENDER);
	isVisible = true;

	피격 = new BitmapComponent(GetOwner());
	피격->SetSprite(L"TestResource/Leak.png");
	// 마나부족 = _owner->CreateComponent<BitmapComponent>();
}

GCameraComponent::~GCameraComponent()
{
}

void GCameraComponent::Trigger피격()
{
	if (피격Timer <= 0)
	{
		피격Timer = 2.f;
	}
}

void GCameraComponent::Trigger마나부족()
{
	// 마나부족->Trigger(true);
}

void GCameraComponent::BeginPlay()
{
	__super::BeginPlay();
	boss = GetOwner()->GetWorld()->FindActorByType<Boss>();
	if (!boss)
	{
		boss = GetOwner()->GetWorld()->FindActorByType<Scarecrow>();
	}
	Math::Vector2 parentPos = parent->GetComponentLocation();
	Math::Vector2 bossPos = boss->rootComponent->GetComponentLocation();
	prevBossPos = bossPos;
	initialDistance = (parentPos - bossPos).Length();

}

void GCameraComponent::Update(float _dt)
{
	if (피격Timer > 0)
	{
		피격Timer -= _dt;
		if ( (int)피격Timer % 2 == 0)
			피격opacity -= _dt;
		else
			피격opacity += _dt;

		// 클램핑
		피격opacity = (std::max)(피격opacity, 0.f);
		피격opacity = (std::min)(1.f, 피격opacity);
	}
}

void GCameraComponent::PostUpdate(float _dt)
{
	CameraComponent::PostUpdate(_dt);

	Math::Vector2 parentPos = parent->GetComponentLocation();

	//if (isMove)
	//{
	//	moveSecond -= _dt;
	//	Math::Vector2 destinationCameraPos = GetComponentLocation() - ((prevBossPos - prevPlayerPos) * 0.5f);
	//	destinationCameraPos.Normalize();
	//	Translate(destinationCameraPos * moveSpeed * _dt);
	//	if (moveSecond < 0.f)
	//		isMove = false;
	//	return;
	//}

	Math::Vector2 bossPos = boss->rootComponent->GetComponentLocation();

	//if ((prevBossPos - bossPos).Length() > 100.f)
	//{
	//	isMove = true;
	//	moveSecond = 1.f;
	//	moveSpeed = ((prevBossPos - bossPos) - GetComponentLocation()).Length() / ((cameraScale * camerScaleRatio > camerScaleMin) ? 1.f + (cameraScale * camerScaleRatio) : 1.f + camerScaleMin);	// (prevBossPos - bossPos).Length() / moveSecond;
	//	prevBossPos = bossPos;
	//	prevPlayerPos = parentPos;
	//	return;
	//}

	//prevBossPos = bossPos;
	Math::Vector2 destinationCameraPos = (bossPos - parentPos) * 0.5f;

	//Scale
	cameraScale = (parentPos - bossPos).Length() / initialDistance;
	cameraScale -= 1;
	cameraScale *= camerScaleRatio;
	if (cameraScale < camerScaleMin)
		cameraScale = camerScaleMin;
	SetScale(1.f + cameraScale, 1.f + cameraScale);


	//Position
	SetTranslation(0, 0);
	Translate(destinationCameraPos);

}

void GCameraComponent::Render(D2DRenderer* _renderer)
{
	
	ID2D1HwndRenderTarget* _renderTarget = _renderer->GetRenderTarget();

	ID2D1DeviceContext* deviceContext;
	_renderTarget->QueryInterface(
		__uuidof(ID2D1DeviceContext),
		(void**)&deviceContext
	);

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	// 레이어 만들기
	ID2D1Layer* layer = nullptr;
	deviceContext->CreateLayer(&layer);
	deviceContext->PushLayer(
		D2D1::LayerParameters(),
		layer
	);

	D2D1_RECT_F dest{
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT
	};
	deviceContext->DrawBitmap(
		피격->GetSprite()->GetResource(),
		&dest,
		피격opacity
	);

	// 레이어 부시기
	deviceContext->PopLayer();
	SafeRelease(&layer);
	
}
