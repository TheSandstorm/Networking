#pragma once
#include "Object.h"
#include "ShaderLoader.h"
#include "Bullet.h"
#include "Ai Movement.h"
#include "AudioManager.h"

class CPlayer : public CModelObject
{
public:
	CPlayer(glm::vec3 _Pos);
	~CPlayer();

	void Update(GLfloat deltaTime, CAudioManager AudioSystem);
	void CreateBullet(glm::vec3 Velocity);
	std::vector<std::shared_ptr<CBullet>>& GetBulletVect() { return BulletVect; };
	glm::vec3& GetTarget() { return Target; };

	bool bShoot;
	Object_Attributes State;
private:
	std::vector<std::shared_ptr<CBullet>> BulletVect;
	void Render();
	glm::vec3 Target;
	float BulletCooldown;
	glm::vec3 BulletVelocity;
	float BulletTimer;
};