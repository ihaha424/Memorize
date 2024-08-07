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

    void AddUICanvas(class Canvas* canvas);
    void RemoveUICanvas(class Canvas* canvas);

    Math::Vector2 ScreenToWorldPoint(Math::Vector2 position);
		
		bool CheckComponentOverlapMulti(
			std::vector<OverlapResult>& outOverlapResults,
			class PrimitiveComponent* primComp,
			const Math::Vector2& pos) const;

		bool CheckComponentOverlapMultiByChannel(
			std::vector<OverlapResult>& outOverlapResults,
			class PrimitiveComponent* primComp,
			const Math::Vector2& pos,
			ECollisionChannel channel) const {
			// TODO
			return false;
		}

		bool CheckComponentSweepMulti(
			std::vector<HitResult>& outHitResults,
			class PrimitiveComponet* primComp,
			const Math::Vector2& start,
			const Math::Vector2& end) const {
			// TODO
			return false;
		}

		bool CheckComponentSweepMultiByChannel(
			std::vector<HitResult>& outHitResults,
			class PrimitiveComponet* primComp,
			const Math::Vector2& start,
			const Math::Vector2& end,
			ECollisionChannel channel) const {
			// TODO
			return false;
		}

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

    /**
     * @brief 메인 카메라 컴포넌트를 설정합니다. 
     * @param component 메인 카메라 컴포넌트
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

    std::vector<class Canvas*> activeUICanvasList;

		// Collision System
		CollisionSystem collisionSystem;
};