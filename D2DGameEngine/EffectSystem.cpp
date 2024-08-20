#include "EffectSystem.h"
#include "World.h"
#include "CameraComponent.h"
#include "D2DRenderer.h"

void EffectSystem::BeginPlay()
{
	for (auto effect : effectList)
	{
		if (effect->GetStatus() == OS_AWAKE)
			effect->BeginPlay();
	}
}

void EffectSystem::FixedUpdate(float _fixedRate)
{
	for (auto effect : effectList)
	{
		if (effect->CheckTickProperty(TICK_PHYSICS) && effect->GetStatus() == OS_ACTIVE)
		{
			effect->FixedUpdate(_fixedRate);
		}
	}
}

void EffectSystem::Destroy()
{
	for (auto effect : effectList)
	{
		if (effect->GetStatus() == OS_DESTROY)
		{
			effect->SetStatus(OS_CLEAN_UP);
		}
	}
}

void EffectSystem::CleanUp()
{
	effectList.remove_if([](IEffect* effect) {
		if (effect->GetStatus() == OS_CLEAN_UP) {
			delete effect;  // 메모리 해제
			return true;    // 리스트에서 제거
		}
		return false;       // 리스트에 남김
		});
}

void EffectSystem::Clean()
{
	for (auto& pEffect : effectList)
	{
		delete pEffect;
	}
	effectList.clear();
}

void EffectSystem::Update(float _dt)
{

	for (auto effect : effectList)
	{
		if (effect->CheckTickProperty(TICK_UPDATE) && effect->GetStatus() == OS_ACTIVE)
		{
			effect->Update(_dt);
		}
	}
}

void EffectSystem::Render(D2DRenderer* _renderer)
{
	Math::Matrix cameraTF = GetWorld()->GetMainCamera()->GetWorldTransform();
	cameraTF = cameraTF * Math::Matrix::CreateTranslation(-(CameraComponent::screenSize.x * 0.5f) * cameraTF._11, -(CameraComponent::screenSize.y * 0.5f) * cameraTF._22, 0.f);
	_renderer->PushTransform(cameraTF.Invert());

	for (auto effect : effectList)
	{
		if (effect->CheckTickProperty(TICK_RENDER) && effect->GetStatus() == OS_ACTIVE)
		{
			if (GetWorld()->GetMainCamera()->InCameraArea(effect))
				effect->Render(_renderer);
		}
	}
	_renderer->PopTransform();
}
