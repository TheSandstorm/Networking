#include "CameraManager.h"
//Init vaules
std::shared_ptr<CCamera> CCamera::CameraPtr;
glm::mat4 CCamera::VPMat;
glm::mat4 CCamera::view;
glm::mat4 CCamera::proj;
glm::vec3 CCamera::camTar;
glm::vec3 CCamera::camLookDir;
glm::vec3 CCamera::camUpDir;
glm::vec3 CCamera::newPos;
glm::vec3 CCamera::camPos = { 0.0f, 0.0f, 0.0f };

//initialize the Camera at the start
CCamera::CCamera()
{
	camPos = glm::vec3(0.0f, 0.0f, 3.0f);
	camLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	camUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(camPos, camTar, camUpDir);
	proj = glm::perspective(45.0f, (float)Utility::SCR_WIDTH / (float)Utility::SCR_HEIGHT, 0.1f, 20000.0f);
}

CCamera::~CCamera()
{
	CameraPtr = nullptr;
}

void CCamera::calculate(float deltaTime)
{
	camPos = glm::vec3(0.0f, 0.0f, 15.0f);
}
//Gives back View and matrix
glm::mat4 CCamera::getVPMat()
{
	return VPMat;
}

// Updates the camera's current position
void CCamera::Update(float deltaTime)
{
	timeElapsed += deltaTime;
	GLfloat radius = 2.0f;
	camPos.x = sin(timeElapsed) * radius;
	camPos.y = 2;
	camPos.z = cos(timeElapsed) * radius;
	camPos = glm::vec3(0.0f, 0.0f, 15.0f);
	view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), camUpDir);
}
//creates the Camera and its pointer
std::shared_ptr<CCamera> CCamera::GetInstance() 
{
	if (CameraPtr == nullptr) CameraPtr = std::shared_ptr<CCamera>(new CCamera);
	return CameraPtr;
}
////Destorys the Camera and its pointer
void CCamera::DestroyInstance()
{
	CameraPtr = nullptr;
}

glm::mat4& CCamera::GetMatrix() {

	glm::mat4 RotationMatrixX =
		glm::rotate(
			glm::mat4(),
			glm::radians(0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 RotationMatrixY =
		glm::rotate(
			glm::mat4(),
			glm::radians(0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

	glm::mat4 TranslationMatrix =
		glm::translate(
			glm::mat4(),
			camPos
		);

	VPMat = proj * view;
	return VPMat;
}