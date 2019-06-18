#pragma once
#include "Object.h"
class CBullet : public CObject
{
public:
	CBullet(glm::vec3 _Velocity, glm::vec3 _Pos);
	CBullet(glm::vec3 _Velocity, glm::vec3 _Pos, glm::vec3 _Scale);
	void Process(float _DeltaTime);
private:
	float MaxSpeed;
};