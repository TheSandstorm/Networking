#pragma once
#include <glew.h>
#include <glm.hpp>
#include <SOIL.h>
#include <vector>
#include "CameraManager.h"

class CubeMap {
public:
	CubeMap();
	~CubeMap();
	void Render(GLuint _Program);
	GLuint GetTexture() { return Texture; };

private:
	GLuint VAO;
	GLuint Texture;
};