#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Utility.h"
#include <freeglut.h>
#include <iostream>

class CCamera
{
public:
	~CCamera();
	static std::shared_ptr<CCamera> GetInstance();
	static void DestroyInstance();
	static void calculate(float deltaTime);
	static glm::mat4 getVPMat();
	void Update(float deltaTime);
	static glm::vec3& GetPos() { return camPos; };
	static glm::mat4& GetMatrix();
	

private:

	CCamera();
	static std::shared_ptr<CCamera> CameraPtr;

	static glm::vec3 camPos;
	static glm::vec3 camLookDir;
	static glm::vec3 camUpDir;

	glm::mat4 rotMat;
	glm::mat4 rotMat2;
	static glm::vec3 newPos;

	static glm::mat4 view;
	static glm::vec3 camTar;

	static glm::mat4 VPMat;
	static glm::mat4 proj;

	float timeElapsed;

};