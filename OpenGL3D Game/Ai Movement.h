#pragma once
#include "Object.h"

class AiMove {
public:
	static glm::vec3 Seek(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _Target);
	static glm::vec3 Seek(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _Target, float _MaxSpeed, float MaxForce);
	static glm::vec3 Pursue(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _TargetPos, glm::vec3 _TargetVelocity);
	static glm::vec3 Wander(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, float _MaxSpeed, float _MaxForce);

private:
	static glm::vec3 Limit(glm::vec3 _Vec, float _MaxForce);
	static glm::vec3 Location;
	static glm::vec3 Velocity;
	static glm::vec3 Acceleration;

	static float MaxSpeed;
	static float MaxForce;
	static float ApproachDistance;
	static float WanderRadius;
};