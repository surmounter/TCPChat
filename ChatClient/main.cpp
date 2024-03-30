#include "TCPClient.h"
#pragma once

#include "TCPClient.h"
#include <sstream>
#include <iostream>

int main()
{
	TCPClient client("127.0.0.1", 9000);
	if (client.Init())
	{
		client.GetOnReceived() = [](const std::string& message)
		{
			std::cout << message << std::endl;
		};
		if (client.Run())
		{
			std::string messageToSend;
			while (client.IsConnected())
			{
				std::getline(std::cin, messageToSend);
				client.Send(messageToSend);
			}
		}
	}
	return 0;
}