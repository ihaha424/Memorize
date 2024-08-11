#pragma once
#include "D2DGameEngine/Actor.h"

class DarkSphereSpawner : public Actor
{
	int projectileCount = 5;
	int nowUsingCount = 0;
	std::vector<class Projectile*> projectiles;
	float speed;
	Math::Vector2 direction;

	float period = 2.f;
	float elapsedTime = 0.f;

	class Player* player = nullptr;
	class GPlayerController* playerController = nullptr;

public:
	DarkSphereSpawner(World* _world);
	virtual ~DarkSphereSpawner();
	
	virtual void Update(float _dt) override;

	void SetSpeed(float _speed) { speed = _speed; }
	void SetPlayer(class Player* _player) { player = _player; }
	void SetPlayerController(class GPlayerController* _playerController) { playerController = _playerController; }
	void Excute(Math::Vector2 _direction);
};

