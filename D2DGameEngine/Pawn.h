#pragma once
#include "Actor.h"
#include "SimpleMath.h"

class Pawn : public Actor
{
	class Controller* controller = nullptr;

public:
	Pawn();
	virtual ~Pawn();

	/**
	 * @brief 컨트롤러를 설정합니다.
	 * @param _controller 
	 */
	virtual void PossessedBy(class Controller* _controller);
};

