#include "GameManager.h"

std::shared_ptr<CGameManager> CGameManager::SceneManagerPtr;
GameState CGameManager::CurrentState;

CGameManager::CGameManager()
{
	//Clears the screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	srand(static_cast<int>(time(NULL)));
	CClock::GetInstance();
	deltaTime = CClock::GetDeltaTime();
	CCamera::GetInstance();
	CObjectManager::GetInstance();
	CLightManager::GetInstance();
	CInputManager();
	CServerManager::GetInstance();
	if (AudioSystem.AudioInit() != true)
	{
		std::cout << "Audio Failed" << std::endl;
		system("pause");
	}
	AudioSystem.LoadAudio(Utility::MenuMusic.data(), 0, FMOD_LOOP_NORMAL);
	AudioSystem.LoadAudio(Utility::ShootingFX.data() , 1, FMOD_LOOP_OFF);
	AudioSystem.LoadAudio(Utility::BackgroundMusic.data(), 2, FMOD_LOOP_NORMAL);
	PlaySound = true;
	SwitchSound = true;

	//For Culling the objects
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	//Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 0.6f, 0.0f, 1.0f); //Orange
	Score = 0;
	Lives = 3;

	//Creates the program for most models
	NormLightProgram3D = ShaderLoader::CreateProgram("Resources/Shaders/First Shader.vs", "Resources/Shaders/First Frag.fs");
	CubeShader = ShaderLoader::CreateProgram(Utility::CubeMapShaderVert.data(), Utility::CubeMapShaderFrag.data());

	CurrentState = START_MENU;
	
	ScoreText = std::make_shared<CTextLabel>("SCORE: 0", Utility::NormalFontString.data(), glm::vec2(-380.0f, 350.0f));
	Title = std::make_shared<CTextLabel>("Tank Busters", Utility::NormalFontString.data(), glm::vec2(-120.0f, 230.0f));
	EndGameTitle = std::make_shared<CTextLabel>("You Died!", Utility::NormalFontString.data(), glm::vec2(90.0f, 100.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	MultiplayerTitle0 = std::make_shared<CTextLabel>("Multiplayer", Utility::NormalFontString.data(), glm::vec2(-90.0f, 350.0f));
	MultiplayerTitle1 = std::make_shared<CTextLabel>("Server List", Utility::NormalFontString.data(), glm::vec2(90.0f, -70.0f));
	
	LivesText = std::make_shared<CTextLabel>("LIVES: 3", Utility::NormalFontString.data(), glm::vec2(200.0f, 350.0f));
	WaitingForParty = std::make_shared<CTextLabel>("Waiting for Party...", Utility::NormalFontString.data(), glm::vec2(-90.0f, -70.0f));


#pragma region StartMenu
	std::vector<std::string> StartOpt;
	StartOpt.push_back("Singleplayer");
	StartOpt.push_back("Multiplayer");
	StartOpt.push_back("Options");
	StartOpt.push_back("Quit");

	StartMenu = std::make_shared<CMenu>(StartOpt, glm::vec2(-90.0f, -200.0f));
#pragma endregion

#pragma region EndMenu
	std::vector<std::string> EndOpt;
	EndOpt.push_back("Retry");
	EndOpt.push_back("Main Menu");
	EndOpt.push_back("Quit");

	EndMenu = std::make_shared<CMenu>(EndOpt, glm::vec2(-90.0f, -200.0f));
#pragma endregion

#pragma region OptionMenu
	std::vector<std::string> OptOpt;
	OptOpt.push_back(""); //Placeholder For BGM Text
	OptOpt.push_back("Back");

	OptionMenu = std::make_shared<CMenu>(OptOpt, glm::vec2(-90.0f, -280.0f));
#pragma endregion

#pragma region MultiplayerMenu
	std::vector<std::string> MultiOpt;
	MultiOpt.push_back("Host Lobby");
	MultiOpt.push_back("Join Lobby");
	MultiOpt.push_back("Back");

	MultiplayerMenu = std::make_shared<CMenu>(MultiOpt, glm::vec2(-90.0f, -250.0f));
#pragma endregion

#pragma region ServerList
	std::vector<std::string> ServerListOpt;
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("----");
	ServerListOpt.push_back("Back");
	ServerListMenu = std::make_shared<CMenu>(ServerListOpt, glm::vec2(-90.0f, 100.0f));
#pragma endregion
	
	Player0 = std::make_shared<CPlayer>(glm::vec3(0.0f,0.0f, 0.0f));
	Player1 = std::make_shared<CPlayer>(glm::vec3(0.0f, 0.0f, 0.0f));
	Floor = std::make_shared<CFloor>(glm::vec3(0.0f, 0.0f, -100.0f));

	TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 14.0f));
	ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.05f, 0.05f, 0.05f));
	RotationMatrix = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	MenuTank = CObjectManager::GetModel(SEEK_ENEMY);
}

void CGameManager::DrawMenu()
{
	MenuTank->Render(TranslationMatrix * RotationMatrix * ScaleMatrix);
	CCamera::GetInstance()->Update(deltaTime);
	CM.Render(CubeShader);
	Floor->Update(deltaTime);
	ScoreText->Render();
	Title->Render();
	StartMenu->Render();
}

void CGameManager::DrawGame()
{
	CM.Render(CubeShader);
	Player0->Update(deltaTime, AudioSystem);
	CCamera::GetInstance()->Update(deltaTime);
	for (auto it : EnemyVect) it->Update(deltaTime);
	Floor->Update(deltaTime);
	ScoreText->Render();
	LivesText->Render();
}

void CGameManager::DrawEnd()
{
	CM.Render(CubeShader);
	MenuTank->Render(TranslationMatrix * RotationMatrix * ScaleMatrix);
	CCamera::GetInstance()->Update(deltaTime);
	Floor->Update(deltaTime);
	ScoreText->SetPosition(glm::vec2(90.0f, 50.0f));
	ScoreText->Render();
	EndGameTitle->Render();
	EndMenu->Render();
}

void CGameManager::DrawOption()
{
	CM.Render(CubeShader);
	CCamera::GetInstance()->Update(deltaTime);
	MenuTank->Render(TranslationMatrix * RotationMatrix * ScaleMatrix);
	Floor->Update(deltaTime);
	ScoreText->Render(); 
	Title->Render();
	OptionMenu->Render();
}

void CGameManager::DrawServerOption() {
	CM.Render(CubeShader);
	Floor->Update(deltaTime);
	ScoreText->Render();
	MultiplayerTitle0->Render();
	MultiplayerMenu->Render();
	
}

void CGameManager::DrawHostLobby() {
	CM.Render(CubeShader);
	Floor->Update(deltaTime);
	ScoreText->Render();
	MultiplayerTitle0->Render();
	WaitingForParty->Render();
	
}

void CGameManager::DrawClientLobby() {
	CM.Render(CubeShader);
	Floor->Update(deltaTime);
	ScoreText->Render();
	MultiplayerTitle0->Render();
	MultiplayerTitle1->Render();
	ServerListMenu->Render();
}

void CGameManager::DrawServerSelect() {
	CM.Render(CubeShader);
	Floor->Update(deltaTime);
	ScoreText->Render();
	MultiplayerTitle0->Render();
	MultiplayerTitle1->Render();
	ServerListMenu->Render();
	
}

void CGameManager::DrawMultiplayerGame() {
	Player0->Update(deltaTime, AudioSystem);
	Player1->Update(deltaTime, AudioSystem);
	for (auto it : EnemyVect) it->Update(deltaTime);
	CM.Render(CubeShader);
	Floor->Update(deltaTime);
	ScoreText->Render();
	LivesText->Render();
}

void CGameManager::GameLoop()
{
	//If the player still exists

	if (Lives == 1 && Player0->State == DEAD)
	{
		CurrentState = END_MENU;
		return;
	}
	if (Player0->State == DEAD)
	{
		EnemyVect.clear();

		Player0->State = NONE;
		Player0->GetPos() = glm::vec3(0.0f, 0.0f, 0.0f);
		Player0->GetVelocity() = glm::vec3(0.0f, 0.0f, 0.0f);
		Player0->GetBulletVect().clear();
		Lives--;
		LivesText->SetText("LIVES: " + std::to_string(Lives));
		return;
	}
	if (Player0 != nullptr) {
		//Spawning Enemies
		if (EnemyVect.size() < (unsigned)MaxEnemys) 
		{
			SpawnTimer += 10.0f * deltaTime;
			if (SpawnTimer >= SpawnLimiter) {
				SpawnTimer = 0.0f;
				glm::vec3 SpawnPos = {
					static_cast<float>((rand() % 6000) - 3000),
					static_cast<float>((rand() % 6000) - 3000),
					0.0f
				};
				if ((rand() % 5) == 0) {
					EnemyVect.push_back(std::make_shared<CEnemyPursue>(SpawnPos, Player0));
				}
				else {
					EnemyVect.push_back(std::make_shared<CEnemyWander>(SpawnPos, Player0));
				}
			}
		}
		CInputManager::ProcessKeyInput(Player0);

		//Checking every entity
		for (unsigned int i = 0; i < EnemyVect.size(); ++i) {
			//Check against every bullet
			for (unsigned int j = 0; j < Player0->GetBulletVect().size(); ++j) {
				if (abs(glm::distance(Player0->GetBulletVect()[j]->GetPos(), EnemyVect[i]->GetPos())) <= EnemyVect[i]->HitRadius) {
					//Destroy if colliding with a bullet
					EnemyVect.erase(EnemyVect.begin() + i);
					Player0->GetBulletVect().erase(Player0->GetBulletVect().begin() + j);
					Score += 10;
					ScoreText->SetText("SCORE: " + std::to_string(Score));

					if (Score == 100 * Counter)
					{
						MaxEnemys += 5;
						Counter++;
					}
					//Add an sound effect
					break;
				}
			}
		}
	}
}

CGameManager::~CGameManager()
{
	CCamera::DestroyInstance();
	CObjectManager::DestroyInstance();
	CLightManager::DestroyInstance();
	CServerManager::GetInstance()->StopNetworkEntity();
	CServerManager::DestroyInstance();
}

void CGameManager::Update()
{
	deltaTime = CClock::GetDeltaTime();
	if (PlaySound == true && SwitchSound == true)
	{
		AudioSystem.PlayAudioBackground(2);
		SwitchSound = false;
	}
	else if (PlaySound == false)
	{
		AudioSystem.StopAudio();
	}

	switch (CurrentState) {

	case START_MENU: {
		int TempOutput = NULL;
		StartMenu->Process(TempOutput);
		CInputManager::ProcessKeyInput();

		switch (TempOutput) {
		case 0:
			CurrentState = SINGLEPLAYER_GAME;
			break;
		case 1:
			CurrentState = MULTIPLAYER_MENU;
			break;
		case 2:
			CurrentState = OPTION_MENU;
			break;
		case 3:
			glutLeaveMainLoop();
			break;
		default:break;
		}
		break;
	}

	case SINGLEPLAYER_GAME:
	{
		GameLoop();
		break;
	}

	case OPTION_MENU:
	{
		//Switches the text if the bgm is on or off
		if (PlaySound == true) OptionMenu->ReplaceOption(0, "Background Music: ON");
		else OptionMenu->ReplaceOption(0, "Background Music: OFF");

		int TempOutput = NULL;
		OptionMenu->Process(TempOutput);
		CInputManager::ProcessKeyInput();

		switch (TempOutput) {
		case 0:
			ToggleMusic();
			break;
		case 1:
			CurrentState = START_MENU;
			break;
		default:break;
		}
		break;
	}

	case END_MENU: 
	{
		int TempOutput = NULL;
		EndMenu->Process(TempOutput);
		CInputManager::ProcessKeyInput();
		switch (TempOutput)
		{
		case 0:
			CurrentState = SINGLEPLAYER_GAME;
			RestartGame();
			break;
		case 1:
			CurrentState = START_MENU;
			RestartGame();
			break;
		case 2:
			glutLeaveMainLoop();
			break;
		default:break;
		}

		break;
	}

	case MULTIPLAYER_MENU: {
		int TempOutput = NULL;
		MultiplayerMenu->Process(TempOutput);
		CInputManager::ProcessKeyInput();

		switch (TempOutput) {
		case 0: {
			std::cout << "Host Selected\n";
			CServerManager::GetInstance()->StartNetwork(SERVER);
			CurrentState = HOST_LOBBY;
			break;
		}
		case 1: {
			std::cout << "Client Selected\n";
			CServerManager::GetInstance()->StartNetwork(CLIENT);
			CurrentState = CLIENT_LOBBY;
			break;
		}
		case 2: {
			CurrentState = START_MENU;
			return;
		}
		default:break;
		}
		break;
	}

	case HOST_LOBBY: {
		CInputManager::ProcessKeyInput(Player0);
		CServerManager::GetInstance()->ProcessNetworkEntity();
		if (CServerManager::GetInstance()->LobbyReady()) {
			CurrentState = HOST_GAME;
		}
		//Player1->GetPos() = ServerManager::GetInstance()->GetPlayerPos();
		break;
	}

	case HOST_GAME: {
		CInputManager::ProcessKeyInput(Player0);
		CServerManager::GetInstance()->ProcessNetworkEntity();
		std::cout << CServerManager::GetInstance()->GetPlayerPos().x;
		Player1->GetPos() = CServerManager::GetInstance()->GetPlayerPos();
		break;
	}

	case CLIENT_GAME: {
		GameLoop();
		std::stringstream ss;
		ss << std::fixed << std::setprecision(1);
		ss << Player0->GetPos().x << ' ' << Player0->GetPos().y;

		TPacket Data;
		Data.Serialize(PLAYERPOS, const_cast<char*>(ss.str().c_str()));
		CServerManager::GetInstance()->SendPacket(Data.PacketData);
		break;
	}

	case CLIENT_LOBBY: {
		//Getting the server list and putting them into the menu options
		CServerManager::GetInstance()->ProcessNetworkEntity();
		int NumServers = CServerManager::GetInstance()->GetServerAddrs().size();
		for (int i = 0; i < NumServers; ++i) {
			ServerListMenu->ReplaceOption(i, ToString(CServerManager::GetInstance()->GetServerAddrs()[i]));
		}
		int TempOutput = NULL;
		ServerListMenu->Process(TempOutput);
		CInputManager::ProcessKeyInput();

		if (TempOutput < NumServers) {
			CServerManager::GetInstance()->SelectServer(TempOutput);

			CurrentState = CLIENT_GAME;
		}
		else if (TempOutput == 6) {
			CServerManager::GetInstance()->StopNetworkEntity();
			CurrentState = MULTIPLAYER_MENU;
		}
		break;

	}
	default: break;
	}
}

void CGameManager::Render()
{
	//Clears the screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//Switches states
	switch (CurrentState) 
	{
	case START_MENU:
		DrawMenu();
		break;
	case SINGLEPLAYER_GAME:
		DrawGame();
		break;
	case END_MENU:
		DrawEnd();
		break;
	case OPTION_MENU:
		DrawOption();
		break;
	case MULTIPLAYER_MENU:
		DrawServerOption();
		break;
	case HOST_LOBBY:
		DrawHostLobby();
		break;
	case CLIENT_GAME:
		DrawMultiplayerGame();
		break;
	case HOST_GAME:
		DrawMultiplayerGame();
		break;
	case CLIENT_LOBBY:
		DrawClientLobby();
		break;
	default:break;
	}
}

void CGameManager::DestroyInstance() 
{
	SceneManagerPtr = nullptr;
}

std::shared_ptr<CGameManager> CGameManager::GetInstance()
{
	if (SceneManagerPtr == nullptr) SceneManagerPtr = std::shared_ptr<CGameManager>(new CGameManager());
	return SceneManagerPtr;
}

void CGameManager::RestartGame() 
{
	EnemyVect.clear();

	Player0->State = NONE;
	Player0->GetPos() = glm::vec3(0.0f, 0.0f, 0.0f);
	Player0->GetVelocity() = glm::vec3(0.0f, 0.0f, 0.0f);
	Player0->GetBulletVect().clear();

	Score = 0;
	Lives = 3;
	ScoreText->SetPosition(glm::vec2(-380.0f, 350.0f));
	ScoreText->SetText("SCORE: " + std::to_string(Score));
	LivesText->SetText("LIVES: " + std::to_string(Lives));
}
//Toggles music on and off
void CGameManager::ToggleMusic() 
{
	if (PlaySound == true) 
	{
		PlaySound = false;
		SwitchSound = true;
	}
	else
	{
		PlaySound = true;
		SwitchSound = true;
	}
};