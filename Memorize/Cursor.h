#pragma once

#include "common.h"

#include "D2DGameEngine/Actor.h"

enum class CursorShape {
	Arrow,
	Pointer,
	Click,
	NUM_SHAPES
};

class Cursor : public Actor
{
public:
	CursorShape shape;
	class SceneComponent* location;
	class AnimationBitmapComponent* arrow;	// default pointer
	class AnimationBitmapComponent* pointer;
	class AnimationBitmapComponent* click;

public:

	Cursor(class World* _world);

	void SetShape(CursorShape _shape);

	void Update(float _dt) override;

	void Render(class D2DRenderer* _renderer) override;
};

