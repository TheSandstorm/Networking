#pragma once

#include "Object.h"
#include "Ai Movement.h"
#include "Player.h"
class CEnemyPursue : public CModelObject
{
public:
	CEnemyPursue(glm::vec3 _Pos, std::shared_ptr<CPlayer> targetEntity);
	~CEnemyPursue();

	void Update(float deltaTime);

private:
	std::shared_ptr<CPlayer> TargetEntity;
	std::vector<std::shared_ptr<CBullet>> BulletVect;
	float ShootTimer;
	float ShootCooldown;
};

class CEnemyWander : public CModelObject
{
public:
	CEnemyWander(glm::vec3 _Pos, std::shared_ptr<CPlayer> targetEntity);
	~CEnemyWander();

	void Update(float deltaTime);
private:
	std::shared_ptr<CPlayer> TargetEntity;
};