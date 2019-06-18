#include "Ai Movement.h"
glm::vec3 AiMove::Location = { 0.0f, 0.0f, 0.0f };
glm::vec3 AiMove::Velocity = { 0.0f, 0.0f, 0.0f };
glm::vec3 AiMove::Acceleration = { 0.0f, 0.0f, 0.0f };
float AiMove::MaxSpeed = 50.0f;
float AiMove::MaxForce = 1.0f;
float AiMove::ApproachDistance = 300.0f;
float AiMove::WanderRadius = 300.0f;
float PI = 3.14159265359f;

//Name:				Limit Function
//Parameters:		Vector, MaxForce
//Return Type:		glm::vec3 Vector
//Description:		Takes a vector and performs a limiting function on it
glm::vec3 AiMove::Limit(glm::vec3 _Vec, float _MaxForce) {
	if (_Vec == glm::vec3(0.0f, 0.0f, 0.0f)) {
		return glm::vec3();
	}
	glm::vec3 v = _Vec;
	if (glm::length(v) > _MaxForce) {
		v = glm::normalize(_Vec) * _MaxForce;
	}
	return v;
}

//Name:				Seek
//Parameters:		Object position, Object velocity, Target vector
//Return Type:		glm::vec3 Steering Velocity
//Description:		Takes an objects position and velocity and calculates the
//                             required vector to reach the target point
glm::vec3 AiMove::Seek(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _Target) {
	//Get the objects current Velocity
	Velocity = _ObjVelocity;

	//Calculated the desired velocity based on the object's position and the target position
	glm::vec3 DesiredVelocity = _Target - _ObjPos;

	//Calculating the distance from the object to the target via finding the length of the DesiredVelocity vect
	float Distance = glm::length(DesiredVelocity);

	//Normalizing
	DesiredVelocity = glm::normalize(DesiredVelocity) * MaxSpeed;

	//If the distance to the target is less than the approach distance (is in "approach" mode)
	if (Distance < ApproachDistance) {
		//Reduce the speed
		DesiredVelocity *= (Distance / ApproachDistance);
	}

	//Calculating the max force that would be applied to the object
	glm::vec3 Steering = DesiredVelocity - _ObjVelocity;
	Steering = Limit(Steering, MaxForce);
	//Setting the z component to 0.0f as we don't need it  
	Steering.z = 0.0f;
	return (Steering);
}

//Name:				Seek (Overload)
//Parameters:		ObjectPos, ObjVelocity, Target, MaxSpeed, MaxForce
//Return Type:		glm::vec3 Steering Velocity
//Description:		Overload of the Seek function to take max speed and max force as well
glm::vec3 AiMove::Seek(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _Target, float _MaxSpeed, float _MaxForce) {
	Velocity = _ObjVelocity;
	glm::vec3 DesiredVelocity = _Target - _ObjPos;
	float Distance = glm::length(DesiredVelocity);
	DesiredVelocity = glm::normalize(DesiredVelocity) * _MaxSpeed;
	if (Distance < ApproachDistance) DesiredVelocity *= (Distance / ApproachDistance);
	glm::vec3 Steering = DesiredVelocity - _ObjVelocity;
	Steering = Limit(Steering, _MaxSpeed);
	Steering.z = 0.0f;
	return (Steering);
}

//Name:				Pursue
//Parameters:		Object Position, ObjectVelocity
//Return Type:		glm::vec3 Vector to the target
//Description:		Generates a vector towards a point that the input object
//							is travelling towards
glm::vec3 AiMove::Pursue(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _TargetPos, glm::vec3 _TargetVelocity) {
	glm::vec3 Target = _TargetPos + (glm::normalize(_TargetVelocity) * 300.0f);
	return Seek(_ObjPos, _ObjVelocity, Target);
}

//Name:				Wander
//Parameters:		Object Position, ObjectVelocity, MaxSpeed, MaxForce
//Return Type:		glm::vec3 Vector to the target
//Description:		The wander behavior. Generates a point on a circle ahead of the object, and seeks
//							towards the point.
glm::vec3 AiMove::Wander(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, float _MaxSpeed, float _MaxForce) {
	//Calculating a set amount in front of the object
	glm::vec3 ForwardVector = _ObjPos + (glm::normalize(_ObjVelocity) * 1000.0f);

	if (_ObjPos.x <= -2800.0f || _ObjPos.x >= 2800.0f || _ObjPos.y >= 2800.0f || _ObjPos.y <= -2800.0f) {
		return Seek(_ObjPos, _ObjVelocity, glm::vec3());
	}
	//Using the x/y values of the forward vector and using parametric calculations, calculate a point on a circle
	glm::vec3 TargetPoint;

	//Finding a random angle (in radians)
	float Theta = (rand() % 360) * (PI / 180.0f);

	//Calculating the x/y with respect to the WanderRadius
	TargetPoint.x = ForwardVector.x + (800.0f * std::cos(Theta));
	TargetPoint.y = ForwardVector.y + (800.0f * std::sin(Theta));
	return Seek(_ObjPos, _ObjVelocity, TargetPoint);
}