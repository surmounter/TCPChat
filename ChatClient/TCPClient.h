#pragma once

#include <string>
#include <thread>
#include <WS2tcpip.h>
#include <functional>
#pragma comment (lib, "ws2_32.lib")

struct Thread
{
	std::thread thread_;
	bool isRunning_ = false;
};

class TCPClient
{
public:
	TCPClient(const std::string& serverIP, const int serverPort);
	virtual ~TCPClient();
public:
	bool Init();
	bool Run();
	bool Send(const std::string& message);
	bool IsConnected() const { return isConnected_; }
	using OnReceived = std::function<void(const std::string&)>;
	OnReceived& GetOnReceived() { return onReceived_; }
private:
	bool isConnected_ = false;
	sockaddr_in serverAddress_;
	SOCKET serverSocket_;
	Thread recvThread_;
	OnReceived onReceived_;
};