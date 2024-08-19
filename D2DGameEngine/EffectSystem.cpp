#include "EffectSystem.h"
#include "World.h"
#include "CameraComponent.h"
#include "D2DRenderer.h"

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
	for (auto actor : effectList)
	{
		if (actor->GetStatus() == OS_DESTROY)
		{
			actor->SetStatus(OS_CLEAN_UP);
		}
	}
}

void EffectSystem::CleanUp()
{
	auto iter = std::_Erase_remove_if(effectList,
		[=](Actor* actor) {
			if (actor->GetStatus() == OS_CLEAN_UP)
			{
				delete actor;
				return 1;
			}
			return 0;
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
	for (int i = 0; i < effectList.size(); i++)
	{
		if (effectList[i]->CheckTickProperty(TICK_UPDATE) && effectList[i]->GetStatus() == OS_ACTIVE)
		{
			effectList[i]->Update(_dt);
		}
	}
}

void EffectSystem::Render(D2DRenderer* _renderer)
{
	Math::Matrix cameraTF = GetWorld()->GetMainCamera()->GetWorldTransform();
	cameraTF = cameraTF * Math::Matrix::CreateTranslation(-CameraComponent::screenSize.x / 2, -CameraComponent::screenSize.y / 2, 0.f);
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
