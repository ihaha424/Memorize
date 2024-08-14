#pragma once

#include "BitmapComponent.h"

// 프레임 데이터 스트럭
struct FrameData {
	float ulx, uly;
	float lrx, lry;
	float cx, cy;
	float duration;
};

class AnimationBitmapComponent : public BitmapComponent {
	bool isLoop;
	bool isTriggered{ false };
	bool isPlaying{ false };
	
	int currentFrameIndex{ 0 };
	std::vector<D2D_RectF> frames;
	std::vector<float> frameDurations;

	float currentFrameTime{ 0.f };

public:
	AnimationBitmapComponent(Actor* _owner) :
		AnimationBitmapComponent(false, _owner) {}

	AnimationBitmapComponent(bool isLoop, Actor* _owner) :
		BitmapComponent(_owner),
		isLoop{ isLoop } {
		AddTickProperties(TICK_UPDATE);
	}

	~AnimationBitmapComponent() {}

	void SetLoop(bool loop) {
		isLoop = loop;
	}

	/**
	 * @brief 스프라이트 시트를 일정한 간격으로 슬라이스 하고, 기본 시간 간격을
	 * 1초로 설정합니다.
	 * @param pixelSizeX 각 프레임의 X 사이즈
	 * @param pixelSizeY 각 프레임의 Y 사이즈
	 * @param offsetX 슬라이싱이 시작되는 우측 상단 코너 부터의 X 오프셋
	 * @param offsetY 슬라이싱이 시작되는 우측 상단 코너 부터의 Y 오프셋
	 * @param paddingX 각 프레임 여분의 패딩 X 값
	 * @param paddingY 각 프레임 여분의 패딩 Y 값
	 */
	void SliceSpriteSheet(
		uint pixelSizeX, uint pixelSizeY,
		int offsetX, int offsetY,
		int paddingX, int paddingY
	);

	/**
	 * @brief 각 애니메이션 프레임의 시간 간격을 설정합니다. 만약, 파라미터로 들어온 
	 * 숫자의 수가 모든 프레임의 수보다 적으면, 마지막으로 입력된 시간 간격
	 * 값이 남은 프레임의 시간 간격으로 적용 됩니다.
	 * 예를 들어, 프레임이 10개가 있다고 가정했을 때, 다음의 코드 실행 시,
	 * `SetFrameDurations({ 0.1 });`
	 * 10 개의 프레임 모두 시간 간격이 0.1초로 설정됩니다.
	 * @param durations 시간 간격 입력값들의 리스트
	 */
	void SetFrameDurations(std::initializer_list<float> durations);

	/**
	 * @brief 스프라이트 프레임 데이터를 직접 입력합니다.
	 * @param ilist 각각의 프레임 데이터 스트럭의 리스트
	 */
	void SetFrameData(std::initializer_list<FrameData> ilist);

	/**
	 * @brief 애니메이션을 초기화 하고, 시작합니다.
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
	 * @brief 애니메이션을 잠시 멈춥니다.
	 */
	void Pause() {
		isPlaying = false;
	}

	/**
	 * @brief 애니메이션이 현재 플레이되고 있는지 확인합니다.
	 * @return True - 현재 애니메이션이 플레이되고 있습니다.
	 */
	bool IsPlaying() {
		return isPlaying;
	}

	/**
	 * @brief 애니메이션을 중단하고 프레임 인덱스를 리셋합니다.
	 */
	void Quit() {
		isTriggered = false;
		isPlaying = false;
		currentFrameIndex = 0;
		currentFrameTime = 0.f;
	}

	virtual void Update(float _dt);
};
