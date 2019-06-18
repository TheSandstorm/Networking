//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School
//
// File Name	: server.cpp
// Description	: Server side of the chat room
// Author		: John Strange
// Mail			: john.str8228@mediadesign.school.nz
//

//Library Includes

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>

//Local Includes
#include "utils.h"
#include "network.h"
#include "consoletools.h"
#include "socket.h"


//Local Includes
#include "server.h"

CServer::CServer()
	:m_pcPacketData(0),
	m_pServerSocket(0)
{
	ZeroMemory(&m_ClientAddress, sizeof(m_ClientAddress));
}

CServer::~CServer()
{
	delete m_pConnectedClients;
	m_pConnectedClients = 0;

	delete m_pServerSocket;
	m_pServerSocket = 0;

	delete m_pWorkQueue;
	m_pWorkQueue = 0;
	
	delete[] m_pcPacketData;
	m_pcPacketData = 0;
}

bool CServer::Initialise()
{
	m_pcPacketData = new char[MAX_MESSAGE_LENGTH];
	
	//Create a work queue to distribute messages between the main  thread and the receive thread.
	m_pWorkQueue = new CWorkQueue<std::pair<sockaddr_in, std::string>>();

	//Create a socket object
	m_pServerSocket = new CSocket();

	//Get the port number to bind the socket to
	unsigned short _usServerPort = QueryPortNumber(DEFAULT_SERVER_PORT);

	//Initialise the socket to the local loop back address and port number
	if (!m_pServerSocket->Initialise(_usServerPort))
	{
		return false;
	}

	//Qs 2: Create the map to hold details of all connected clients
	m_pConnectedClients = new std::map < std::string, TClientDetails >() ;

	return true;
}

bool CServer::AddClient(std::string _strClientName)
{
	//TO DO : Add the code to add a client to the map here...
	
	for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
	{
		//Check to see that the client to be added does not already exist in the map, 
		if(it->first == ToString(m_ClientAddress))
		{
			return false;
		}
		//also check for the existence of the username
		else if (it->second.m_strName == _strClientName)
		{
			return false;
		}
	}

	std::cout << "Client Being Added to Server\n Current Users: ";
	for (const auto& it : *m_pConnectedClients) {
		std::cout << it.first << " ";
	}
	std::cout << std::endl;

	//Add the client to the map.
	TClientDetails _clientToAdd;
	_clientToAdd.m_strName = _strClientName;
	_clientToAdd.m_ClientAddress = this->m_ClientAddress;

	std::string _strAddress = ToString(m_ClientAddress);
	m_pConnectedClients->insert(std::pair < std::string, TClientDetails > (_strAddress, _clientToAdd));
	LobbyReady = true;
	return true;
}

bool CServer::SendData(char* _pcDataToSend)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;
	
	int iNumBytes = sendto(
		m_pServerSocket->GetSocketHandle(),				// socket to send through.
		_pcDataToSend,									// data to send
		_iBytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet target
		sizeof(m_ClientAddress)							// size of the above address struct.
		);
	//iNumBytes;
	if (_iBytesToSend != iNumBytes)
	{
		std::cout << "There was an error in sending data from client to server" << std::endl;
		return false;
	}
	return true;
}

bool CServer::SendDataTo(char* _pcDataToSend, sockaddr_in _clientAdrress)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;

	int iNumBytes = sendto(
		m_pServerSocket->GetSocketHandle(),				// socket to send through.
		_pcDataToSend,									// data to send
		_iBytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr*>(&_clientAdrress),	// address to be filled with packet target
		sizeof(_clientAdrress)							// size of the above address struct.
	);
	//iNumBytes;
	if (_iBytesToSend != iNumBytes)
	{
		std::cout << "There was an error in sending data from client to server" << std::endl;
		return false;
	}
	return true;
}

void CServer::ReceiveData(char* _pcBufferToReceiveData)
{
	
	int iSizeOfAdd = sizeof(m_ClientAddress);
	int _iNumOfBytesReceived;
	int _iPacketSize;

	//Make a thread local buffer to receive data into
	char _buffer[MAX_MESSAGE_LENGTH];

	while (true)
	{
		// pull off the packet(s) using recvfrom()
		_iNumOfBytesReceived = recvfrom(			// pulls a packet from a single source...
			m_pServerSocket->GetSocketHandle(),						// client-end socket being used to read from
			_buffer,							// incoming packet to be filled
			MAX_MESSAGE_LENGTH,					   // length of incoming packet to be filled
			0,										// flags
			reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet source
			&iSizeOfAdd								// size of the above address struct.
		);
		if (_iNumOfBytesReceived < 0)
		{
			int _iError = WSAGetLastError();
			ErrorRoutines::PrintWSAErrorInfo(_iError);
			//return false;
		}
		else
		{
			_iPacketSize = static_cast<int>(strlen(_buffer)) + 1;
			strcpy_s(_pcBufferToReceiveData, _iPacketSize, _buffer);
			char _IPAddress[100];
			inet_ntop(AF_INET, &m_ClientAddress.sin_addr, _IPAddress, sizeof(_IPAddress));
			
			std::cout << "Server Received \"" << _pcBufferToReceiveData << "\" from " <<
				_IPAddress << ":" << ntohs(m_ClientAddress.sin_port) << std::endl;
			//Push this packet data into the WorkQ
			m_pWorkQueue->push(std::make_pair(m_ClientAddress,_pcBufferToReceiveData));
		}
		//std::this_thread::yield();
		
	} //End of while (true)
}

void CServer::GetRemoteIPAddress(char *_pcSendersIP)
{
	char _temp[MAX_ADDRESS_LENGTH];
	int _iAddressLength;
	inet_ntop(AF_INET, &(m_ClientAddress.sin_addr), _temp, sizeof(_temp));
	_iAddressLength = static_cast<int>(strlen(_temp)) + 1;
	strcpy_s(_pcSendersIP, _iAddressLength, _temp);
}

unsigned short CServer::GetRemotePort()
{
	return ntohs(m_ClientAddress.sin_port);
}

bool CServer::SendtoAll(char * _pcDataToSend)
{
	for (auto it1 : *m_pConnectedClients)
	{
		m_ClientAddress = it1.second.m_ClientAddress;
		SendData(_pcDataToSend);
	}
	return true;
}

bool CServer::SendtoAllExceptUser(char * _pcDataToSend, sockaddr_in _clientAdrress)
{
	for (unsigned int i = 0; i < m_pConnectedClients->size(); i++)
	{
		int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;

		int iNumBytes = sendto(
			m_pServerSocket->GetSocketHandle(),				// socket to send through.
			_pcDataToSend,									// data to send
			_iBytesToSend,									// number of bytes to send
			0,												// flags
			reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet target
			sizeof(m_ClientAddress)							// size of the above address struct.
		);
		//iNumBytes;
		if (_iBytesToSend != iNumBytes)
		{
			std::cout << "There was an error in sending data from client to server" << std::endl;
			return false;
		}
	}
	return true;
}

void CServer::ProcessData(std::pair<sockaddr_in, std::string> dataItem)
{
	TPacket _packetRecvd, _packetToSend;
	_packetRecvd = _packetRecvd.Deserialize(const_cast<char*>(dataItem.second.c_str()));
	int currentNumServer = (int)m_pConnectedClients->size();
	switch (_packetRecvd.MessageType)
	{
	case HANDSHAKE:
	{
		//Set up check for forign data
		std::string message = ("Number of Users in chatroom : " + std::to_string(currentNumServer));
		std::cout << "Server received a handshake message " << std::endl;
		if (AddClient(_packetRecvd.MessageContent))
		{
			//Qs 3: To DO : Add the code to do a handshake here

			_packetToSend.Serialize(HANDSHAKE_SUCCESS, const_cast<char*>(message.c_str()));
			SendDataTo(_packetToSend.PacketData, dataItem.first);
		}
		else
		{
			// If username in use try again
			_packetToSend.Serialize(HANDSHAKE_FAILED, const_cast<char*>("Handshake Failed"));
			SendDataTo(_packetToSend.PacketData, dataItem.first);
		}
		break;
	}
	case DATA:
	{
		//Q4 it processes if ! is at the start of the message
		std::cout << "Server received a data message" << std::endl;
		std::string clientName;
		std::string message = _packetRecvd.MessageContent;
		auto it = m_pConnectedClients->find(ToString(m_ClientAddress));
		//if the map can't find the user: error
		if (it == m_pConnectedClients->end()) 
		{
			std::cout << "User was not found" << std::endl;
		}
		else if (message.substr(1,6) == "!q")
		{
			//Quit packet
			TPacket quitPacket;
			message = "You have left the server";
			quitPacket.Serialize(DATA, const_cast<char*>(message.c_str()));
		
			std::string username(const_cast<char*>(dataItem.second.c_str()));
			//Tells the server that the user had left
			message = "[SERVER] " + username + " has left";
			for (auto itQuit : *m_pConnectedClients) {
				_packetToSend.Serialize(DATA, const_cast<char*>(message.c_str()));
				SendtoAll(_packetToSend.PacketData);
			}
			//removes the user from the Client list
			m_pConnectedClients->erase(ToString(m_ClientAddress));
		}
		else if (message.substr(1, 6) == "!?")
		{
			//Command list
			_packetToSend.Serialize(DATA, const_cast<char*>("Avaliable Commands are : !q = Quit"));
			SendDataTo(_packetToSend.PacketData, dataItem.first);		
		}
		else
		{
			clientName = it->second.m_strName;
			clientName.erase(0, 1);
			message = "[" + clientName + "]" + _packetRecvd.MessageContent;
			_packetToSend.Serialize(DATA, const_cast<char*>(message.c_str()));
			SendtoAll(_packetToSend.PacketData);
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));	
		break;
	}

	case BROADCAST:
	{
		std::cout << "Received a broadcast packet" << std::endl;
		//Just send out a packet to the back to the client again which will have the server's IP and port in it's sender fields
		_packetToSend.Serialize(BROADCAST, const_cast<char*>("I'm here!"));
		SendData(_packetToSend.PacketData);
		break;
	}
	//Keep Alive function
	case KEEPALIVEC:
	{
		std::string message = "<SERVER INFO>"; 
		_packetToSend.Serialize(KEEPALIVEC, const_cast<char*>(message.c_str()));
		SendData(_packetToSend.PacketData);
		break;
	}

	case KEEPALIVES:
	{
		m_pConnectedClients->at(ToString(m_ClientAddress)).m_bIsAlive = true; //It's alive
		break;
	}

	case PLAYERPOS: {
		std::string str(dataItem.second.c_str());
		std::stringstream ss(str);
		std::cout << ss.str() << std::endl;
		float x, y;
		ss >> x >> y;
		Player1Pos = glm::vec3(x, y, -0.2f);
	}

	default:
		break;

	}
}

CWorkQueue<std::pair<sockaddr_in, std::string>>* CServer::GetWorkQueue()
{
	return m_pWorkQueue;
}

void CServer::CheckPulse()
{
	TPacket _packet;
	std::string message = "Ping";
	for (std::map<std::string, TClientDetails>::const_iterator it = (*m_pConnectedClients).begin(); it != (*m_pConnectedClients).end(); ++it)
	{
			_packet.Serialize(KEEPALIVES, const_cast<char*>(message.c_str()));
			(*m_pConnectedClients)[it->first].m_bIsAlive = false;
			SendDataTo(_packet.PacketData, (*m_pConnectedClients)[it->first].m_ClientAddress);
	}
}
//Needs to Check each user and see who is not alive and then disconnect them
void CServer::Dropoff()
{
}
