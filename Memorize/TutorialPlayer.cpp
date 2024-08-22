#include "TutorialPlayer.h"
#include "TutorialTextBitmap.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"

TutorialPlayer::TutorialPlayer(World* _world) : Player(_world)
{
	orbText = CreateComponent<TutorialTextBitmap>();
	orb->AddChild(orbText);
	orbText->SetTranslation(150, -90);

}

TutorialPlayer::~TutorialPlayer()
{
}
