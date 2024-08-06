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

AnimationBitmapComponent* Animator::GetCurrentAnimationScene() {
	return _currState->_animation;
}

void Animator::Update(float _dt)
{
	GetCurrentAnimationScene()->Update(_dt);
}

void Animator::Render(D2DRenderer* _renderer)
{
	GetCurrentAnimationScene()->Render(_renderer);
}
