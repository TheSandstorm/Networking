#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>

//Holds the light info for the shaders and does the calucations 
class CLightManager
{
public:
	~CLightManager();
	static std::shared_ptr<CLightManager> GetInstance();
	static void DestroyInstance();
	static float& getAmbientStr() { return ambientStr; };
	static glm::vec3& getAmbientColor() { return ambientColor; };
	static glm::vec3& getLightColor() { return lightColor; };
	static glm::vec3& getLightPos() { return lightPos; };
	glm::mat3 ConvertedModelMatrix(glm::mat4 model);

private:
	CLightManager();
	static std::shared_ptr<CLightManager> LightManagerptr;
	static float ambientStr;
	static glm::vec3 ambientColor;
	static glm::vec3 lightColor;
	static glm::vec3 lightPos;
};