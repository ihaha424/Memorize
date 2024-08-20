#include "framework.h"
#include "World.h"
#include "Level.h"
#include "UIPanel.h"
#include "CameraComponent.h"
#include "Canvas.h"
#include "PrimitiveComponent.h"

World::World() 
{
	canvas = new Canvas(this);
	effectSystem.SetWorld(this);
}
World::~World()
{
	auto iter = levelList.begin();
	for (; iter != levelList.end(); iter++)
	{
		delete iter->second;
	}
	levelList.clear();
	effectSystem.Clean();
}

void World::ChangeScene()
{
	if (NextLevel)
	{
		if (CurLevel)
		{
			CurLevel->Exit();
			CurLevel->Clear();
			effectSystem.Clean();
		}
		CurLevel = NextLevel;
		NextLevel = nullptr;
		CurLevel->Enter();
	}
}

void World::SetNextScene(std::wstring nextLevel)
{
	const auto iter = levelList.find(nextLevel);
	assert(iter != levelList.end());
	NextLevel = iter->second;
}


Math::Vector2 World::ScreenToWorldPoint(Math::Vector2 position)
{
	Math::Matrix cameraTF = GetMainCamera()->GetWorldTransform();
	Math::Vector2 center = {CameraComponent::screenSize.x * 0.5f , CameraComponent::screenSize.y * 0.5f};
	Math::Vector2 ScreenPos = (position - center);
	ScreenPos.x = ScreenPos.x * cameraTF._11;
	ScreenPos.y = ScreenPos.y * cameraTF._22;
	Math::Vector2 temp = ScreenPos + Math::Vector2{mainCamera->GetComponentLocation().x, mainCamera->GetComponentLocation().y};

	return temp;
}

bool World::CheckComponentOverlapMulti(
	std::vector<OverlapResult>& outOverlapResults,
	class PrimitiveComponent* primComp,
	const Math::Vector2& pos,
	const Math::Matrix& rotation) {
	CheckComponentOverlapMultiByChannel(outOverlapResults, primComp, pos, rotation, primComp->GetCollisionObjectType());
	return (outOverlapResults.size() > 0);
}

bool World::CheckComponentOverlapMultiByChannel(
	std::vector<OverlapResult>& outOverlapResults, 
	PrimitiveComponent* primComp, 
	const Math::Vector2& pos, 
	const Math::Matrix& rotation, 
	ECollisionChannel channel)
{
	if (!primComp) {
		return false;
	}

	if (primComp->IsZeroExtent()) {
		// TODO: do raycast instead.
		return false;
	}

	CollisionShape collisionShape;
	if (!primComp->GetCollisionShape(1.0, collisionShape)) {
		return false;
	}

	if (collisionSystem.CheckCollisionShapeOverlapsMulti(outOverlapResults, primComp, collisionShape, pos, rotation, channel, primComp->collisionProperty))
	{
		return true;
	}

	return false;
}

bool World::CheckComponentSweepMulti(std::vector<HitResult>& outHitResults, PrimitiveComponent* primComp, const Math::Vector2& start, const Math::Vector2& end, const Math::Matrix& rotation)
{
	bool bBlockingHit = CheckComponentSweepMultiByChannel(outHitResults, primComp, start, end, rotation, primComp->GetCollisionObjectType());
	return bBlockingHit;
}

bool World::CheckComponentSweepMultiByChannel(
	std::vector<HitResult>& outHitResults, 
	PrimitiveComponent* primComp, 
	const Math::Vector2& start, 
	const Math::Vector2& end, 
	const Math::Matrix& rotation, 
	ECollisionChannel channel)
{
	if (!primComp || ((start - end).LengthSquared() <= (4.f * EPSILON) * (4.f * EPSILON))) {
		return false;
	}

	if (primComp->IsZeroExtent()) {
		// TODO: do raycast instead.
		return false;
	}

	if (!primComp->IsCollisionEnabled()) return false;

	CollisionShape collisionShape;
	if (!primComp->GetCollisionShape(1.0, collisionShape)) {
		return false;
	}

	if (collisionSystem.CheckCollsionShapeSweepMulti(outHitResults, primComp, collisionShape, start, end, rotation, channel, primComp->collisionProperty))
	{
		return true;
	}

	return false;
}

void World::UpdateCollisionSystem()
{
	if (!CurLevel) return;

	collisionSystem.collisionMap.clear();

	for (Actor* actor : CurLevel->actorList)
	{
		if (actor->GetStatus() != EObjectStatus::OS_ACTIVE)
			continue;

		for (auto [_, component] : actor->components)
		{
			if (component->GetStatus() != EObjectStatus::OS_ACTIVE)
				continue;

			PrimitiveComponent* primComp = dynamic_cast<PrimitiveComponent*>(component);
			if (primComp == nullptr)
				continue;

			if (primComp->bCanCollide)
			{
				RegisterComponentCollision(primComp);
			}
		}
	}
}

void World::BeginPlay()
{
	UpdateCollisionSystem();
	CurLevel->BeginPlay();
}

void World::FixedUpdate(float _fixedRate)
{
	CurLevel->FixedUpdate(_fixedRate);
	CurLevel->CleanUp();
	CurLevel->Destroy();

	effectSystem.FixedUpdate(_fixedRate);
	effectSystem.CleanUp();
	effectSystem.Destroy();
}

void World::PreUpdate(float _dt)
{
	CurLevel->PreUpdate(_dt);
}

void World::Update(float _dt)
{
	CurLevel->Update(_dt);

	effectSystem.Update(_dt);

	canvas->Update(_dt);

}

void World::PostUpdate(float _dt)
{
	CurLevel->PostUpdate(_dt);
	ChangeScene();
}

void World::Render(D2DRenderer* _renderer)
{
	// ·»´õ ¼ø¼­ Á¤·Ä
	CurLevel->PrepareRender();

	// ÇöÀç ·¹º§ ·»´õ
	CurLevel->Render(_renderer);

	//Effect ·£´õ
	effectSystem.Render(_renderer);

	// UI ·»´õ
	canvas->Render(_renderer);

}