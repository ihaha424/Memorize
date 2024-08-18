#pragma once

#include "BitmapComponent.h"

// ������ ������ ��Ʈ��
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
	bool isReverse{ false };
	
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
	void FrameResize(int index){
		frames.resize(index);
		frameDurations.resize(index);
	}
	void SetReverse(bool _reverse) {
		isReverse = _reverse;
	}

	std::size_t GetFrameSize() {
		return frames.size();
	}

	void FrameSplice(int first, int last)
	{
		if (first < 0 || GetFrameSize() < last)
		{
			LOG_ERROR(-1, "Invalid splicing range!");
			return;
		}

		auto framesFirst = frames.begin();
		std::advance(framesFirst, first);
		auto framesLast = frames.end();
		std::advance(framesLast, last);
		frames = std::vector<D2D_RectF>(framesFirst, framesLast);

		auto frameDurationsFirst = frameDurations.begin();
		std::advance(frameDurationsFirst, first);
		auto frameDurationsLast = frameDurations.end();
		std::advance(frameDurationsLast, last);
		frameDurations = std::vector<float>(frameDurationsFirst, frameDurationsLast);
	}

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
		InitFrame();
		isTriggered = trigger;
		isPlaying = trigger;
		if (!trigger) {
			if (!isReverse)
				currentFrameIndex = 0;
			else
				currentFrameIndex = frames.size() - 1;
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
	 * @brief �ִϸ��̼��� �ٽ� �����մϴ�.
	 */
	void Resume() {
		isPlaying = true;
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
		currentFrameTime = 0.f;
		if(!isReverse)
			currentFrameIndex = 0;
		else
			currentFrameIndex = frames.size() - 1;
	}

	virtual void Update(float _dt);

	void InitFrame();
};
