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
}

AnimationState* Animator::GetCurrentAnimationScene() {
	return _currState;
}

void Animator::Update(float _dt)
{
	_currState->Update(_dt);
}

void Animator::Render(D2DRenderer* _renderer)
{
	_currState->Render(_renderer);
}
