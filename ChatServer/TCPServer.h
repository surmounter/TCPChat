#pragma once

#include <string>
#include <set>
#include <queue>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

class TCPServer
{
public:
	TCPServer(const std::string& ipAddress, const int port);
	virtual ~TCPServer();
public:
	bool Init();
	bool Run();
private:
	sockaddr_in address_;
};
