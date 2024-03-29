#include <iostream>
#include "TCPServer.h"

int main()
{
	TCPServer server("127.0.0.1", 9000);
	if (server.Init())
	{
		if (server.Run() == false)
		{
			std::cout << WSAGetLastError() << std::endl;
		}
	}
	return 0;
}