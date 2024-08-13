#pragma once

#include "Level.h"

#include "CollisionSystem.h"
#include "CollisionShape.h"
#include "OverlapResult.h"
#include "HitResult.h"

class World
{
public:
    World();
    virtual ~World();

public:
    template<typename T>
    T* CreateLevel(const std::wstring& LevelName)
    {
        bool IsBase = std::is_base_of<Level, T>::value;
        assert(IsBase == true);
        T* pLevel = new T(this, LevelName);
        levelList.insert({ LevelName, pLevel });
        return pLevel;
    }

    void ChangeScene();
    void SetNextScene(std::wstring nextLevel);
    class Level* GetCurLevel() { return CurLevel; }

		template<typename T>
		T* FindActorByType() {
			if (!CurLevel) return nullptr;

			return GetCurLevel()->FindActorByType<T>();
		}

		template<typename T>
		std::vector<T*> FindAllActorsByType() {
			if (!CurLevel) return std::vector<T*>();

			return GetCurLevel()->FindAllActorsByType<T>();
		}

    void AddUICanvas(class Canvas* canvas);
    void RemoveUICanvas(class Canvas* canvas);
    class Canvas* GetCanvas() { return canvas; }

    Math::Vector2 ScreenToWorldPoint(Math::Vector2 position);
		
		bool CheckComponentOverlapMulti(
			std::vector<OverlapResult>& outOverlapResults,
			class PrimitiveComponent* primComp,
			const Math::Vector2& pos,
			const Math::Matrix& rotation);

		bool CheckComponentOverlapMultiByChannel(
			std::vector<OverlapResult>& outOverlapResults,
			class PrimitiveComponent* primComp,
			const Math::Vector2& pos,
			const Math::Matrix& rotation,
			ECollisionChannel channel);

		bool CheckComponentSweepMulti(
			std::vector<HitResult>& outHitResults,
			class PrimitiveComponent* primComp,
			const Math::Vector2& start,
			const Math::Vector2& end,
			const Math::Matrix& rotation);

		bool CheckComponentSweepMultiByChannel(
			std::vector<HitResult>& outHitResults,
			class PrimitiveComponent* primComp,
			const Math::Vector2& start,
			const Math::Vector2& end,
			const Math::Matrix& rotation,
			ECollisionChannel channel);

		bool LineTraceMultiByChannel(
			std::vector<HitResult>& outHitResults,
			const Math::Vector2& start,
			const Math::Vector2& end,
			ECollisionChannel channel,
			const CollisionProperty& collisionProperty) const {
			// TODO
			return false;
		}

		bool LineTraceSingleByChannel(
			HitResult& outHitResults,
			const Math::Vector2& start,
			const Math::Vector2& end,
			ECollisionChannel channel,
			const CollisionProperty& collisionProperty) const {
			// TODO
			return false;
		}

		bool OverlapMultiByChannel(
			std::vector<OverlapResult>& outOverlapResults,
			const Math::Vector2& pos,
			ECollisionChannel channel,
			const CollisionShape& collisionShape,
			const CollisionProperty& collisionProperty) const {
			// TODO
			return false;
		}

		bool SweepMultiByChannel(
			std::vector<HitResult>& outHitResults,
			const Math::Vector2& start,
			const Math::Vector2& end,
			ECollisionChannel channel,
			const CollisionShape& collisionShape,
			const CollisionProperty& collisionProperty) const {
			// TODO
			return false;
		}

		bool SweepSingleByChannel(
			HitResult& outHitResults,
			const Math::Vector2& start,
			const Math::Vector2& end,
			ECollisionChannel channel,
			const CollisionShape& collisionShape,
			const CollisionProperty& collisionProperty) const {
			// TODO
			return false;
		}

		void RegisterComponentCollision(PrimitiveComponent* primComp) {
			collisionSystem.RegisterComponent(primComp);
		}

		void UnregisterComponentCollision(PrimitiveComponent* primComp) {
			collisionSystem.UnregisterComponent(primComp);
		}

		void UpdateComponentCollision(PrimitiveComponent* primComp) {
			collisionSystem.UpdateComponent(primComp);
		}

		bool IsComponentCollisionRegistered(PrimitiveComponent* primComp) {
			return collisionSystem.IsComponentRegistered(primComp);
		}

    /**
     * @brief ���� ī�޶� ������Ʈ�� �����մϴ�. 
     * @param component ���� ī�޶� ������Ʈ
     */
    void SetMainCamera(class CameraComponent* _camera) { mainCamera = _camera; };
    class CameraComponent* GetMainCamera() { return mainCamera; };

    virtual void BeginPlay();
    virtual void FixedUpdate(float _fixedRate);
    virtual void PreUpdate(float _dt);
    virtual void Update(float _dt);
    virtual void PostUpdate(float _dt);
    virtual void Render(class D2DRenderer* _renderer);

protected:
    std::map<std::wstring, Level*>  levelList;
    Level* NextLevel = nullptr;
    Level* CurLevel = nullptr;

    class CameraComponent* mainCamera = nullptr;

	class Canvas* canvas = nullptr;

		// Collision System
		CollisionSystem collisionSystem;
};