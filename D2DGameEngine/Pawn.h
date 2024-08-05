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
	 * @brief 컨트롤러를 반환합니다. 
	 * @return controller
	 */
	class Controller* GetController() const { return controller; };

	/**
	 * @brief 컨트롤러를 설정합니다.
	 * @param _controller
	 */
	void SetController(class Controller* _controller) { controller = _controller; }
};

