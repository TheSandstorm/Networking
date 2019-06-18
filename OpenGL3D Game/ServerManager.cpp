#include "ServerManager.h"

std::shared_ptr<CServerManager> CServerManager::ServerManagerPtr = nullptr;

CServerManager::CServerManager() {
	//Timer
	start = std::chrono::high_resolution_clock::now();
	_pcPacketData = new char[MAX_MESSAGE_LENGTH];
	strcpy_s(_pcPacketData, strlen("") + 1, "");

	NetworkPtr = CNetwork::GetInstance();
	NetworkPtr->StartUp();

	ClientPtr = nullptr;
	ServerPtr = nullptr;

	ZeroMemory(&IPAddressArray, strlen(IPAddressArray));
}

CServerManager::~CServerManager() {
	StopNetworkEntity();
	CNetwork::DestroyInstance();
	ClientPtr = nullptr;
	ServerPtr = nullptr;
	NetworkPtr = nullptr;

	delete[] _pcPacketData;
	_pcPacketData = nullptr;
}

std::shared_ptr<CServerManager> CServerManager::GetInstance() {
	if (ServerManagerPtr == nullptr) ServerManagerPtr = std::shared_ptr<CServerManager>(new CServerManager());
	return ServerManagerPtr;
}

void CServerManager::DestroyInstance() {
	ServerManagerPtr = nullptr;
}

bool CServerManager::SelectServer(unsigned int _Opt) {
	return ClientPtr->SelectServer(_Opt);
}

std::vector<sockaddr_in> CServerManager::GetServerAddrs() {
	return ClientPtr->GetServerList();
}

void CServerManager::StartNetwork(EEntityType _Type) {
	NetworkEntityType = _Type;
	if (!NetworkPtr->GetInstance()->Initialise(NetworkEntityType)) {
		std::cout << "Unable to initialise the Network\n";
		return;
	}
	//Run receive on a separate thread so that it does not block the main client thread.
	if (NetworkEntityType == CLIENT) { //if network entity is a client 
		ClientPtr = static_cast<CClient*>(NetworkPtr->GetInstance()->GetNetworkEntity());
		_ClientReceiveThread = std::thread(&CClient::ReceiveData, ClientPtr, std::ref(_pcPacketData));
	}
	//Run receive of server also on a separate thread 
	else if (NetworkEntityType == SERVER) { //if network entity is a server 
		ServerPtr = static_cast<CServer*>(NetworkPtr->GetInstance()->GetNetworkEntity());
		_ServerReceiveThread = std::thread(&CServer::ReceiveData, ServerPtr, std::ref(_pcPacketData));
	}
}

void CServerManager::ProcessNetworkEntity() {
	if (NetworkEntityType == CLIENT) {//if network entity is a client
		if (ClientPtr != nullptr) {
			//Timer for dropout Client side to check if server is dead
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			auto timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

			if (ClientPtr->bPulse && (timeElapsed > cTimeout))
			{
				ClientPtr->ConnectionAlive = false;
				ClientPtr->CheckPulse();
				ClientPtr->bPulse = false;
				start = std::chrono::high_resolution_clock::now();
			}
			else if (!ClientPtr->bPulse && timeElapsed > cWindow)
			{
				ClientPtr->Dropoff(); //Drop the server due to it not responding
				ClientPtr->bPulse = true;
				start = std::chrono::high_resolution_clock::now();
			}

			//If the message queue is empty 
			if (ClientPtr->GetWorkQueue()->empty())
			{
				//Don't do anything
			}
			else
			{
				//Retrieve off a message from the queue and process it
				std::string temp;
				ClientPtr->GetWorkQueue()->pop(temp);
				ClientPtr->ProcessData(const_cast<char*>(temp.c_str()));
			}
		}
	}
	else { //if you are running a server instance
		if (ServerPtr != nullptr) {
			//Timer for dropout server side to check if client is dead
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			auto timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

			if (bPulse && (timeElapsed > sTimeout))
			{
				ServerPtr->CheckPulse(); //Check connection of the clients
				bPulse = false;
				start = std::chrono::high_resolution_clock::now();
				std::cout << "Sending pulse" << std::endl;
			}
			//Not fully working 
			else if (!bPulse && timeElapsed > sWindow)
			{
				std::cout << "Dropping the client." << std::endl;
				//_pServer->Dropoff(); //Drop any clients that have taken too long to respond
				bPulse = true;
				start = std::chrono::high_resolution_clock::now();
			}

			if (!ServerPtr->GetWorkQueue()->empty())
			{
				NetworkPtr->GetInstance()->GetNetworkEntity()->GetRemoteIPAddress(IPAddressArray);
				//std::cout << _cIPAddress
				//<< ":" << _rNetwork.GetInstance().GetNetworkEntity()->GetRemotePort() << "> " << _pcPacketData << std::endl;

				//Retrieve off a message from the queue and process it
				std::pair<sockaddr_in, std::string> dataItem;
				ServerPtr->GetWorkQueue()->pop(dataItem);
				ServerPtr->ProcessData(dataItem);
			}
		}
	}
}

void CServerManager::SendPacket(std::string _Data) {
	TPacket DataPacket;
	DataPacket.Serialize(PLAYERPOS, const_cast<char*>(_Data.c_str()));
	NetworkPtr->GetNetworkEntity()->SendData(DataPacket.PacketData);
}

void CServerManager::StopNetworkEntity() {
	if (ClientPtr != nullptr) {
		ClientPtr->GetOnlineState() = false;
		if (_ClientReceiveThread.joinable()) _ClientReceiveThread.join();
		ClientPtr = nullptr;
	}

	if (ServerPtr != nullptr) {
		ServerPtr->GetOnlineState() = false;
		if (_ServerReceiveThread.joinable()) _ServerReceiveThread.join();
		ServerPtr = nullptr;
	}
}

glm::vec3 CServerManager::GetPlayerPos() {
	return ServerPtr->Player1Pos;
}

bool CServerManager::LobbyReady() {
	return ServerPtr->LobbyReady;
}