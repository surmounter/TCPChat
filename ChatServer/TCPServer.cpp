#include "TCPServer.h"
#include <winsock2.h>
#include <iostream>
#include <sstream>
#include "SocketSet.h"

TCPServer::TCPServer(const std::string& ipAddress, const int port)
{
	address_.sin_family = AF_INET;
	address_.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &address_.sin_addr);
}

TCPServer::~TCPServer()
{
	WSACleanup();
}

bool TCPServer::Init()
{
	WSADATA data;
	WORD version = MAKEWORD(2,2);
	return WSAStartup(version, &data) == 0;
}

bool TCPServer::Run()
{
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		return false;
	}
	if (bind(listenSocket, (sockaddr*)&address_, sizeof(address_)) == SOCKET_ERROR)
	{
		return false;
	}
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		return false;
	}

	SocketSet socketSet;
	socketSet.Add(listenSocket);
	
	constexpr int bufferSize = 4096;
	char buffer[bufferSize];

	while (true)
	{
		fd_set handleableSockets = socketSet.Get();
		if (select(0, &handleableSockets, nullptr, nullptr, nullptr) == SOCKET_ERROR)
		{
			break;
		}
		socketSet.ForeachHandleableSockets(handleableSockets, [&socketSet, listenSocket, &buffer, bufferSize](auto socket)
		{
			if (socket == listenSocket)
			{
				auto clientSocket = accept(listenSocket, nullptr, nullptr);
				if (clientSocket == SOCKET_ERROR)
				{
					return;
				}
				socketSet.Add(clientSocket);
				static const std::string WelcomeMessage = "Welcome!!";
				send(clientSocket, WelcomeMessage.c_str(), WelcomeMessage.size() + 1, 0);
				std::cout << "New user joined the chat." << std::endl;
			}
			else
			{
				ZeroMemory(buffer, bufferSize);
				const int bytesReceived = recv(socket, buffer, bufferSize, 0);
				if (bytesReceived <= 0)
				{
					socketSet.ReserveRemove(socket);
					return;
				}
				socketSet.ForeachSockets([senderSocket = socket, buffer, bytesReceived, listenSocket](auto socket)
				{
					if (socket == listenSocket)
					{
						return;
					}
					if (socket == senderSocket)
					{
						static const std::string DeliveredMessage = "Message delivered.";
						send(socket, DeliveredMessage.c_str(), DeliveredMessage.size() + 1, 0);
					}
					else
					{
						send(socket, buffer, bytesReceived, 0);
					}
				});
			}
		});
		socketSet.ExecuteRemove();
	}
	return true;
}
