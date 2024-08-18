#include "AnimationBitmapComponent.h"

void AnimationBitmapComponent::SliceSpriteSheet(uint pixelSizeX, uint pixelSizeY, int offsetX, int offsetY, int paddingX, int paddingY)
{
	int newTotalWidth = GetSpriteWidth() - offsetX;
	int newTotalHeight = GetSpriteHeight() - offsetY;

	int nrows = newTotalHeight / (pixelSizeY + paddingY);
	int ncols = newTotalWidth / (pixelSizeX + paddingX);

	frames.resize(nrows * ncols);
	frameDurations.resize(nrows * ncols, 1.0);

	auto idx = [&ncols](int row, int col) -> int {
		return col + row * ncols;
	};

	for (int i = 0; i < nrows; ++i) {
		for (int j = 0; j < ncols; ++j) {
			float left = (float)offsetX + (pixelSizeX + paddingX) * j;
			float top = (float)offsetY + (pixelSizeY + paddingY) * i;
			float right = left + (float)pixelSizeX;
			float bottom = top + (float)pixelSizeY;
			frames[idx(i, j)] = D2D_RectF{
				left,
				top,
				right,
				bottom
			};
		}
	}
}

void AnimationBitmapComponent::SetFrameDurations(std::initializer_list<float> durations)
{
	auto it = durations.begin();
	for (float& duration : frameDurations) {
		if (it == durations.end()) --it;
		duration = *it;
		++it;
	}
}

void AnimationBitmapComponent::SetFrameData(std::initializer_list<FrameData> ilist)
{
	std::size_t numFrames = ilist.size();

	frames.resize(ilist.size());
	frameDurations.resize(ilist.size());

	auto it = ilist.begin();
	for (std::size_t i = 0; i < numFrames; ++i) {
		if (it == ilist.end()) --it;
		frames[i] = { it->ulx, it->uly, it->lrx, it->lry };
		frameDurations[i] = it->duration;
		++it;
	}
}

void AnimationBitmapComponent::Update(float _dt)
{
	if (!isPlaying) return;
	// �ִϸ��̼� ������Ʈ
	currentFrameTime += _dt;
	if (currentFrameTime >= frameDurations[currentFrameIndex]) {
		currentFrameTime -= frameDurations[currentFrameIndex];

		if (!isReverse)
		{
			currentFrameIndex++;
			bShouldUpdateBounds = true;
			if (currentFrameIndex >= frames.size()) {
				if (isLoop) {
					currentFrameIndex = 0;
				}
				else {
					Quit();
				}			
			}
		}
		else	//ReversePlay
		{
			currentFrameIndex--;
			bShouldUpdateBounds = true;
			if (currentFrameIndex < 0) {
				if (isLoop) {
					currentFrameIndex = frames.size() - 1;
				}
				else {
					Quit();
				}
			}
		}
	}

	// ��Ʈ�� ������ ������Ʈ
	D2D_RectF& frame = frames[currentFrameIndex];
	SetFrame(
		{ frame.left, frame.top },
		{ frame.right, frame.bottom }
	);
}

void AnimationBitmapComponent::InitFrame()
{
	if (frames.empty()) return;
	D2D_RectF& frame = frames[0];
	SetFrame(
		{ frame.left, frame.top },
		{ frame.right, frame.bottom }
	);

}
