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
	// 애니메이션 업데이트
	currentFrameTime += _dt;
	if (currentFrameTime >= frameDurations[currentFrameIndex]) {
		currentFrameTime -= frameDurations[currentFrameIndex];

		currentFrameIndex++;
		if (currentFrameIndex >= frames.size()) {
			if (isLoop) {
				currentFrameIndex = 0;
			}
			else {
				Quit();
			}
		}
	}

	// 비트맵 프레임 업데이트
	D2D_RectF& frame = frames[currentFrameIndex];
	SetFrame(
		{ frame.left, frame.top },
		{ frame.right, frame.bottom }
	);
}
