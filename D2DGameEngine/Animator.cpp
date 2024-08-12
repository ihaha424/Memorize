#include "Animator.h"

#include "AnimationBitmapComponent.h"
#include "AnimationState.h"

void Animator::Toggle() {
	_currState->Toggle(this);
}

void Animator::SetState(AnimationState* state) {
	_currState->Exit(this);
	_currState = state;
	_currState->Enter(this);
	// bShouldUpdateBounds = true;
	MarkBoundsDirty();
}

AnimationState* Animator::GetCurrentAnimationScene() const {
	return _currState;
}

BoxCircleBounds Animator::CalculateLocalBounds() const {
	D2D_RectF frame = GetCurrentAnimationScene()->GetCurrFrame();
	std::initializer_list<Math::Vector2> vertices{
		{	frame.left, frame.top	},
		{ frame.right, frame.top },
		{ frame.right, frame.bottom },
		{	frame.left, frame.bottom	}
	};
	TPolygon polygon{ vertices };
	return BoxCircleBounds(polygon.GetAABB());
}

void Animator::Update(float _dt)
{
	_currState->Update(_dt);
}

void Animator::Render(D2DRenderer* _renderer)
{
	_currState->Render(_renderer);
}
