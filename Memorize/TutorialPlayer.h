#pragma once
#include "Player.h"
class TutorialPlayer : public Player
{
public:
	class TutorialTextBitmap* orbText;

	TutorialPlayer(World* _world);
	virtual ~TutorialPlayer();
};

