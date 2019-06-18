#pragma once
#include <fstream>
#include <glew.h>
#include <glm.hpp>
#include <glut.h>
#include <SOIL.h>
#include "ShaderLoader.h"
#include "Utility.h"
#include "Model.h"

enum Object_Attributes
{
	//Object Types
	PLAYER_ENTITY,
	BULLET_ENTITY,
	SEEK_ENEMY,
	WANDER_ENEMY,
	FLOOR_ENTITY,
	//States
	DEAD,
	NONE,
};

struct MESH
{
	GLuint VAO;
	unsigned int IndicesCount;
	GLuint Texture;
	GLuint Shader;
};

class CObjectManager
{
public:
	~CObjectManager();
	static std::shared_ptr<CObjectManager> GetInstance();
	static void DestroyInstance();

	static std::shared_ptr<MESH> GetMesh(Object_Attributes _ObjectType);
	static std::shared_ptr<Model> GetModel(Object_Attributes _ObjectType);

private:
	CObjectManager();

	static std::shared_ptr<CObjectManager> ObjectManagerptr;

	static GLuint WaveShader;
	static GLuint ObjectShader;
	static GLuint ModelShader;
	static GLuint TextShader;

	//Mesh ptrs
	static std::shared_ptr<MESH> CubeMesh;
	static std::shared_ptr<MESH> SphereMesh;

	//Model ptrs
	static std::shared_ptr<Model> Player;
	static std::shared_ptr<Model> Enemy1;
	static std::shared_ptr<Model> Enemy2;

};