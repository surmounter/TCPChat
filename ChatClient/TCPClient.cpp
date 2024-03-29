#include "TCPClient.h"

TCPClient::TCPClient(const std::string& serverIP, const int serverPort)
{
	serverAddress_.sin_family = AF_INET;
	serverAddress_.sin_port = htons(serverPort);
	inet_pton(AF_INET, serverIP.c_str(), &serverAddress_.sin_addr);
}

TCPClient::~TCPClient()
{
	closesocket(serverSocket_);
	WSACleanup();
	if (recvThread_.isRunning_)
	{
		recvThread_.thread_.join();
	}
}

bool TCPClient::Init()
{
	WSADATA data;
	return WSAStartup(MAKEWORD(2,2), &data) == 0;
}

bool TCPClient::Run()
{
	serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket_ == INVALID_SOCKET)
	{
		return false;
	}
	if (connect(serverSocket_, (sockaddr*)&serverAddress_, sizeof(serverAddress_)) == SOCKET_ERROR)
	{
		closesocket(serverSocket_);
		return false;
	}
	recvThread_.thread_ = std::thread([this]
	{
		recvThread_.isRunning_ = true;
		while (recvThread_.isRunning_)
		{
			static constexpr int MaxBufferSize = 4096;
			char buffer[MaxBufferSize];
			ZeroMemory(buffer, MaxBufferSize);
			const int bytesReceived = recv(serverSocket_, buffer, MaxBufferSize, 0);
			if (bytesReceived <= 0)
			{
				continue;
			}
			if (onReceived_)
			{
				onReceived_(std::string(buffer, 0, bytesReceived));
			}
		}
	});
	return true;
}

bool TCPClient::Send(const std::string& message)
{
	if (serverSocket_ == INVALID_SOCKET)
	{
		return false;
	}
	send(serverSocket_, message.c_str(), message.size() + 1, 0);
	return true;
}

