#pragma once
#include <glm.hpp>
#include <iostream>
#include <chrono>
#include "Client.h"
#include "Server.h"
#include "Network.h"

class CServerManager {
public:
	~CServerManager();

	//Singleton Methods
	static std::shared_ptr<CServerManager> GetInstance();
	static void DestroyInstance();

	//Server Methods
	bool SelectServer(unsigned int _Opt);
	std::vector<sockaddr_in> GetServerAddrs();

	void StartNetwork(EEntityType _Type);
	void ProcessNetworkEntity();
	void SendPacket(std::string _Data);
	void StopNetworkEntity();

	glm::vec3 GetPlayerPos();

	bool LobbyReady();


private:
	CServerManager();										//Singleton constructor
	static std::shared_ptr<CServerManager> ServerManagerPtr;	//Singleton pointer

	CClient* ClientPtr;								//Pointer to the client instance (if it is running)
	CServer* ServerPtr;								//Pointer to the server instance (if it is running)

	CNetwork* NetworkPtr;

	EEntityType NetworkEntityType;

	std::thread _ClientReceiveThread;				//Separate thread for client receive so that it does not block main
	std::thread _ServerReceiveThread;				//Separate thread for server receive so that it does not block main

	char* _pcPacketData = 0;						//Packet Send/recieve information
	char IPAddressArray[MAX_ADDRESS_LENGTH];		//Ip Address array

	float TimeSinceLastCheck;						//Keep alive message timer

	bool bPulse = true;
	int sTimeout = 20;
	int sWindow = 5;
	int cTimeout = 20;
	int cWindow = 5;

	std::chrono::high_resolution_clock::time_point start;
};