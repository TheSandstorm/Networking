#pragma once
#ifndef _OBJECT_H__
#define __OBJECT_H__
#include <glew.h>
#include <glm.hpp>
#include <glut.h>
#include <SOIL.h>
#include <vector>
#include "ObjectManager.h"
#include "Utility.h"
#include "CameraManager.h"
class CObject
{
public:
	CObject();
	CObject(Object_Attributes _ObjectType, glm::vec3 _Pos);
	virtual ~CObject();
	virtual void Update(float _deltaTime);
	virtual glm::vec3& GetPos() { return Pos; };
	virtual glm::vec3& GetScale() { return Scale; };
	virtual glm::vec3& GetRotation() { return Rotation; };
	virtual glm::vec3& GetVelocity() { return Velocity; };

	
	Object_Attributes Type;

	void SetPos(glm::vec3 newPos);
private:

protected:
	virtual void Render();

	unsigned IndicesCount;

	GLuint VAO;
	GLuint Shader;
	GLuint Texture;

	glm::vec3 Pos;
	glm::vec3 Scale;
	glm::vec3 Rotation;
	glm::vec3 Velocity;

	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;

};

class CModelObject : public CObject
{
public:
	CModelObject();
	CModelObject(Object_Attributes ObjectType, glm::vec3 Pos);
	virtual void Update(float deltaTime);
	float GetSpeed() { return maxSpeed; };
	float GetForce() { return maxForce; };
	float HitRadius;
protected:
	virtual void Render();
	std::shared_ptr<CObject> TargetObject;
	std::shared_ptr<Model> model;
	float maxSpeed;
	float maxForce;
};
#endif 