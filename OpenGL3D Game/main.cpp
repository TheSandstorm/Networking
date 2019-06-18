#include "GameManager.h"
#include "Utility.h"
#include <iostream>
#include <glew.h>
#include <freeglut.h>

#pragma comment(lib,"ws2_32.lib")
using namespace std;


void Update()
{
	CGameManager::GetInstance()->Update();
	glutPostRedisplay();
}

void Render()
{
	CGameManager::GetInstance()->Render();
	glutSwapBuffers();
}

void Exit() 
{
	CGameManager::DestroyInstance();
	exit(1);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(Utility::SCR_HEIGHT, Utility::SCR_WIDTH);
	glutCreateWindow("OpenGL 3D Game - John Strange");

	//Sets up all gl function callbacks based on pc hardware
	if (glewInit() != GLEW_OK)
	{
		//If glew setup failed thenapplication will not run graphics correctly
		cout << "Glew Initialization Failed. Aborting Application." << endl;
		system("pause");
	}
	CGameManager::GetInstance();

	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutCloseFunc(Exit);
	glutMainLoop();
	return (0);
}