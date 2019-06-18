#include "Bullet.h"

CBullet::CBullet(glm::vec3 _Velocity, glm::vec3 _Pos) 
{
	Pos = _Pos;
	Pos.z += 25.0f;
	Scale = glm::vec3(0.04f, 0.04f, 0.04f);
	Rotation = glm::vec3();
	Velocity = _Velocity;
	MaxSpeed = 1000.0f;
	VAO = CObjectManager::GetMesh(BULLET_ENTITY)->VAO;
	IndicesCount = CObjectManager::GetMesh(BULLET_ENTITY)->IndicesCount;
	Texture = CObjectManager::GetMesh(BULLET_ENTITY)->Texture;
	Shader = CObjectManager::GetMesh(BULLET_ENTITY)->Shader;
	Type = BULLET_ENTITY;
}

CBullet::CBullet(glm::vec3 _Velocity, glm::vec3 _Pos, glm::vec3 _Scale) : CBullet(_Velocity, _Pos)
{
	Scale = _Scale;
}

void CBullet::Process(float _DeltaTime) 
{
	VPMatrix = CCamera::GetMatrix();
	if (Velocity == glm::vec3(0.0f, 0.0f, 0.0f)) return;
	Pos += Velocity * MaxSpeed * _DeltaTime;
	Render();
}