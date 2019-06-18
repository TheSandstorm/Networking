#pragma once
#include "ShaderLoader.h"
#include "AudioManager.h"
#include "Light Manager.h"
#include "Player.h"
#include "Clock.h"
#include "CameraManager.h"
#include "CubeMap.h"
#include "InputManager.h"
#include "TextManager.h"
#include "ServerManager.h"
#include "Menu.h"
#include "Enemy.h"
#include "Floor.h"
#include "Server.h"
#include "Client.h"
#include "utils.h"
#include <glew.h>
#include <glut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iomanip>
class CGameManager
{
public:
	~CGameManager();
	static void DestroyInstance();
	static std::shared_ptr<CGameManager> GetInstance();

	void RestartGame();

	void ToggleMusic();

	void Update();
	void Render();
	static GameState& GetState() { return CurrentState; };

private:
	static std::shared_ptr<CGameManager> SceneManagerPtr;
	CGameManager();

	float SpawnLimiter = 3.0f;
	float SpawnTimer = 0.0f;
	int MaxEnemys = 10;
	int Counter = 1;
	//Parts of the game that can be rendered.
	void DrawMenu();
	void DrawGame();
	void DrawEnd();
	void DrawOption();
	void DrawServerOption();
	void DrawHostLobby();
	void DrawMultiplayerGame();
	void DrawClientLobby();
	void DrawServerSelect();

	void GameLoop();

	std::shared_ptr<CTextLabel> Title;
	std::shared_ptr<CTextLabel> EndGameTitle;
	std::shared_ptr<CTextLabel> ScoreText;
	std::shared_ptr<CTextLabel> LivesText;
	std::shared_ptr<CTextLabel> MultiplayerTitle0;
	std::shared_ptr<CTextLabel> MultiplayerTitle1;
	std::shared_ptr<CTextLabel> WaitingForParty;

	std::vector<std::string> StartOpt;
	std::vector<std::string> MultiOpt;
	std::vector<std::string> OptOpt;
	std::vector<std::string> EndOpt;

	std::shared_ptr<CMenu> StartMenu;
	std::shared_ptr<CMenu> EndMenu;
	std::shared_ptr<CMenu> OptionMenu;
	std::shared_ptr<CMenu> MultiplayerMenu;
	std::shared_ptr<CMenu> ServerListMenu;

	GLfloat deltaTime;
	static GameState CurrentState;
	unsigned int Score;
	int Lives;
	bool PlaySound;
	bool SwitchSound;


	CAudioManager AudioSystem;
	CubeMap CM;
	GLuint CubeShader;
	std::shared_ptr<CFloor> Floor;
	std::shared_ptr<Model> MenuTank;
	std::shared_ptr<CPlayer> Player0;
	//For Networking
	std::shared_ptr<CPlayer> Player1;
	bool _Connected = false;
	bool _ServerChosen = false;
	EEntityType InstanceType;

	std::vector<std::shared_ptr<CModelObject>> EnemyVect;

	GLuint NormLightProgram3D;
	GLuint TextShader;

	glm::mat4 TranslationMatrix;
	glm::mat4 ScaleMatrix;
	glm::mat4 RotationMatrix;
};