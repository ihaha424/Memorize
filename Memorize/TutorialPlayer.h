#pragma once
#include "Player.h"
class TutorialPlayer : public Player
{
public:
	TutorialTextBitmap* orbText;

	TutorialPlayer(World* _world);
	virtual ~TutorialPlayer();
};

