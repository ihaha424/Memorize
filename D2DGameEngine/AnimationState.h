#pragma once

#include "framework.h"
#include "SpriteResource.h"
#include "ResourceManager.h"
#include "SceneComponent.h"

class Animator;
class AnimationState {
	// ������ ������ ��Ʈ��
	struct FrameData {
		float ulx, uly;
		float lrx, lry;
		float cx, cy;
		float duration;
	};

	friend class Animator;
	using StateName = std::string;
	using StateDependency = std::unordered_map<StateName, AnimationState*>;

protected:
	StateDependency _stateDependency;
	SceneComponent* onwer;

public:
	AnimationState(SceneComponent* _owner);
	virtual ~AnimationState() {}

#pragma region _AnimationState
	virtual void Exit(Animator*);
	virtual void Toggle(Animator*);
	virtual void Enter(Animator*);

	void AddAnimationStateDependency(
		const StateName&,
		AnimationState*
	);
#pragma endregion

#pragma region _BitMap
private:
	D2D_RectF frame = {
			0,0,
			GetSpriteWidth(),
			GetSpriteHeight()
	};
	std::shared_ptr<SpriteResource> sprite;

	D2D_Mat3x2F imageTransform;
public:
	void SetSprite(const wchar_t* path) {
		sprite = ResourceManager::LoadResource<SpriteResource>(path);
		if (!sprite->GetResource()) {
			throw std::exception("BitmapScene: image asset is not found!");
		}
		// Fit the frame with the image width and height
	}

	std::shared_ptr<SpriteResource> GetSprite() { return sprite; }

	float GetSpriteWidth() const {
		return sprite->GetResource()->GetSize().width;
	}

	float GetSpriteHeight() const {
		return sprite->GetResource()->GetSize().height;
	}

	float GetFrameWidth() const {
		return frame.right - frame.left;
	}

	float GetFrameHeight() const {
		return frame.top - frame.bottom;
	}

	void SetFrame(const D2D_Point2F& ul, const D2D_Point2F& lr) {
		frame = { ul.x, ul.y, lr.x, lr.y };
	}

	void FlipX(bool flip) {
		imageTransform._11 = flip ? -1.f : 1.f;
	}

	void FlipY(bool flip) {
		imageTransform._22 = flip ? -1.f : 1.f;
	}

	virtual D2D_RectF GetCurrFrame() { return frame; }
#pragma endregion

#pragma region _AnimationBitMap
private:
	bool isLoop;
	bool isTriggered{ false };
	bool isPlaying{ false };

	int currentFrameIndex{ 0 };
	std::vector<D2D_RectF> frames;
	std::vector<float> frameDurations;

	float currentFrameTime{ 0.f };
public:
	void SetLoop(bool _isLoop);

	/**
	 * @brief Frame reSize
	 * @param index 
	 */
	void FrameResize(int index);

		/**
	 * @brief ��������Ʈ ��Ʈ�� ������ �������� �����̽� �ϰ�, �⺻ �ð� ������
	 * 1�ʷ� �����մϴ�.
	 * @param pixelSizeX �� �������� X ������
	 * @param pixelSizeY �� �������� Y ������
	 * @param offsetX �����̽��� ���۵Ǵ� ���� ��� �ڳ� ������ X ������
	 * @param offsetY �����̽��� ���۵Ǵ� ���� ��� �ڳ� ������ Y ������
	 * @param paddingX �� ������ ������ �е� X ��
	 * @param paddingY �� ������ ������ �е� Y ��
	 */
	void SliceSpriteSheet(
		uint pixelSizeX, uint pixelSizeY,
		int offsetX, int offsetY,
		int paddingX, int paddingY
	);

	/**
	 * @brief �� �ִϸ��̼� �������� �ð� ������ �����մϴ�. ����, �Ķ���ͷ� ����
	 * ������ ���� ��� �������� ������ ������, ���������� �Էµ� �ð� ����
	 * ���� ���� �������� �ð� �������� ���� �˴ϴ�.
	 * ���� ���, �������� 10���� �ִٰ� �������� ��, ������ �ڵ� ���� ��,
	 * `SetFrameDurations({ 0.1 });`
	 * 10 ���� ������ ��� �ð� ������ 0.1�ʷ� �����˴ϴ�.
	 * @param durations �ð� ���� �Է°����� ����Ʈ
	 */
	void SetFrameDurations(std::initializer_list<float> durations);

	/**
	 * @brief ��������Ʈ ������ �����͸� ���� �Է��մϴ�.
	 * @param ilist ������ ������ ������ ��Ʈ���� ����Ʈ
	 */
	void SetFrameData(std::initializer_list<FrameData> ilist);

	/**
	 * @brief �ִϸ��̼��� �ʱ�ȭ �ϰ�, �����մϴ�.
	 * @param trigger
	 */
	void Trigger(bool trigger) {
		isTriggered = trigger;
		isPlaying = trigger;
		if (!trigger) {
			currentFrameIndex = 0;
			currentFrameTime = 0.f;
		}
	}

	/**
	 * @brief �ִϸ��̼��� ��� ����ϴ�.
	 */
	void Pause() {
		isPlaying = false;
	}

	/**
	 * @brief �ִϸ��̼��� ���� �÷��̵ǰ� �ִ��� Ȯ���մϴ�.
	 * @return True - ���� �ִϸ��̼��� �÷��̵ǰ� �ֽ��ϴ�.
	 */
	bool IsPlaying() {
		return isPlaying;
	}

	/**
	 * @brief �ִϸ��̼��� �ߴ��ϰ� ������ �ε����� �����մϴ�.
	 */
	void Quit() {
		isTriggered = false;
		isPlaying = false;
		currentFrameIndex = 0;
		currentFrameTime = 0.f;
	}


	void Update(float _dt);
	void Render(class D2DRenderer* _renderer);
#pragma endregion
};

template<class T>
concept AnimationStateType = std::is_base_of<AnimationState, T>::value;
