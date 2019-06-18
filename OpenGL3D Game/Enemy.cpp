#include "Enemy.h"

CEnemyPursue::CEnemyPursue(glm::vec3 _Pos, std::shared_ptr<CPlayer> _TargetEntity)
{
	Scale = glm::vec3(0.03f, 0.03f, 0.03f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	Pos = _Pos;
	Velocity = { 1.0f, 0.0f, 0.0f };
	TargetEntity = _TargetEntity;
	maxForce = 1.0f;
	maxSpeed = 6.0f;
	HitRadius = 65.0f;
	ShootTimer = 0.0f;
	ShootCooldown = 5.0f;
	Type = SEEK_ENEMY;
	model = CObjectManager::GetModel(Type);
	
}

CEnemyPursue::~CEnemyPursue()
{
	BulletVect.clear();
}

void CheckNan(glm::vec3& _Vec) {
	for (int i = 0; i < 3; ++i) {
		if (isnan(_Vec.x)) _Vec.x = 0.0f;
		if (isnan(_Vec.y)) _Vec.y = 0.0f;
		if (isnan(_Vec.z)) _Vec.z = 0.0f;
	}
}

void CEnemyPursue::Update(float deltaTime)
{
	if (TargetEntity == nullptr) return;
	if (ShootTimer >= ShootCooldown) {
		if (glm::distance(Pos, TargetEntity->GetPos()) <= 500.0f) {
			//Getting the dir at which to fire the bullet
			glm::vec3 Dir = TargetEntity->GetPos() - Pos;
			BulletVect.push_back(std::make_shared<CBullet>(glm::normalize(Dir) * 0.4f, Pos, glm::vec3(0.03f, 0.03f, 0.03f)));
			ShootTimer = 0.0f;
		}
	}
	ShootTimer += 10.f * deltaTime;
	VPMatrix = CCamera::GetMatrix();
	Velocity += AiMove::Pursue(Pos, Velocity, TargetEntity->GetPos(), TargetEntity->GetVelocity());
	CheckNan(Velocity);
	Pos += Velocity * maxSpeed * deltaTime;
	Render();

	//Checking that bullets dont go out of range
	for (unsigned int i = 0; i < BulletVect.size(); ++i) {
		if (BulletVect[i]->GetPos().y >= 3300.0f ||
			BulletVect[i]->GetPos().y <= -3300.0f ||
			BulletVect[i]->GetPos().x >= 3300.0f ||
			BulletVect[i]->GetPos().x <= -3300.0f) {
			BulletVect.erase(BulletVect.begin(), BulletVect.begin() + i);
			continue;
		}

		//Check if the player is being hit
		if (glm::distance(BulletVect[i]->GetPos(), TargetEntity->GetPos()) <= TargetEntity->HitRadius) {
			TargetEntity->State = DEAD;
		}
		//Otherwise process the bullets
		BulletVect[i]->Process(deltaTime);
	}
}

CEnemyWander::CEnemyWander(glm::vec3 _Pos, std::shared_ptr<CPlayer> _TargetEntity)
{
	Scale = glm::vec3(0.06f, 0.06f, 0.06f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	Pos = _Pos;
	Velocity = { 1.0f, 0.0f, 0.0f };
	maxForce = 1.0f;
	maxSpeed = 20.0f;
	HitRadius = 130.0f;
	TargetEntity = _TargetEntity;
	Type = WANDER_ENEMY;
	model = CObjectManager::GetModel(Type);
	
}

CEnemyWander::~CEnemyWander()
{
}

void CEnemyWander::Update(float deltaTime)
{
	if (TargetEntity == nullptr) return;
	VPMatrix = CCamera::GetMatrix();
	Velocity += AiMove::Wander(Pos, Velocity, maxSpeed, maxForce);
	Pos += Velocity * maxSpeed * deltaTime;
	Render();

	if (glm::distance(Pos ,TargetEntity->GetPos()) <= TargetEntity->HitRadius) {
		TargetEntity->State = DEAD;
	}
}
