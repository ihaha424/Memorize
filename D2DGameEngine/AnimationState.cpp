#include "AnimationState.h"

#include "Animator.h"
#include "D2DRenderer.h"

AnimationState::AnimationState(SceneComponent* _owner)
	:onwer(_owner),
	frame{ 0.f, 0.f, 0.f, 0.f },
	sprite{ nullptr },
	imageTransform{ D2D_Mat3x2F::Identity() }
{}

void AnimationState::Exit(Animator* controller) {
	// e.g. in the 'PlayerIdle' animation state 
	// 
	// _animation = _animationSceneRegistry["idle_exit"];
	Trigger(false);
}

void AnimationState::Toggle(Animator* controller) {
	// e.g. in the 'PlayerIdle' animation state 
	// 
	// if (controller->GetVariable("Jump") == true) {
	//   controller->SetState(PlayerJump);
	// } else if (controller->GetVariable("Duck") == true) {
	//   controller->SetState(PlayerDuck);
	// }
	//
}

void AnimationState::Enter(Animator* controller) {
	// e.g. in the 'PlayerIdle' animation state 
	// 
	// _animation = _animationSceneRegistry["idle_exit"];
	Trigger(true);
}

void AnimationState::Update(float _dt)
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

void AnimationState::Render(D2DRenderer* _renderer)
{
	_renderer->PushTransform(onwer->GetWorldTransform());

	D2D_RectF dest{
		.left = -GetFrameWidth() / 2.f,
		.top = -GetFrameHeight() / 2.f,
		.right = GetFrameWidth() / 2.f,
		.bottom = GetFrameHeight() / 2.f
	};

	_renderer->DrawSprite(sprite->GetResource(), dest, frame);

#ifndef NDEBUG
	_renderer->DrawBorder(
		{ dest.left, dest.top },
		{ dest.right, dest.bottom },
		D2D_Color::Green
	);
#endif

	_renderer->PopTransform();
}

void AnimationState::AddAnimationStateDependency(
	const StateName& name,
	AnimationState* state) {

	auto it = _stateDependency.find(name);
	if (it != _stateDependency.end()) {
		throw std::invalid_argument(
			"The state \'" + name +
			"\' already exists!");
	}

	_stateDependency.insert(
		std::make_pair(name, state)
	);
}

void AnimationState::SliceSpriteSheet(uint pixelSizeX, uint pixelSizeY, int offsetX, int offsetY, int paddingX, int paddingY)
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

void AnimationState::SetFrameDurations(std::initializer_list<float> durations)
{
	auto it = durations.begin();
	for (float& duration : frameDurations) {
		if (it == durations.end()) --it;
		duration = *it;
		++it;
	}
}

void AnimationState::SetFrameData(std::initializer_list<FrameData> ilist)
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
