#include "Light Manager.h"

std::shared_ptr<CLightManager> CLightManager::LightManagerptr;
glm::vec3 CLightManager::ambientColor;
glm::vec3 CLightManager::lightColor;
glm::vec3 CLightManager::lightPos;
float CLightManager::ambientStr;

CLightManager::CLightManager()
{
	ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	lightPos = glm::vec3(-2.0f, 6.0f, 6.0f);
	ambientStr = 0.05f;
}

std::shared_ptr<CLightManager> CLightManager::GetInstance()
{
	if (LightManagerptr == nullptr) LightManagerptr = std::shared_ptr<CLightManager>(new CLightManager);
	return LightManagerptr;
}
////Destorys the Camera and its pointer
void CLightManager::DestroyInstance()
{
	LightManagerptr = nullptr;
}

glm::mat3 CLightManager::ConvertedModelMatrix(glm::mat4 model)
{
	glm::mat3 ConvertedModel = glm::mat3(glm::transpose(glm::inverse(model)));
	return ConvertedModel;
}

CLightManager::~CLightManager()
{
	LightManagerptr = nullptr;
}

