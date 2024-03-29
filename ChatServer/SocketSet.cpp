#include "SocketSet.h"

SocketSet::SocketSet()
{
	FD_ZERO(&sockets_);
}

void SocketSet::Add(const SOCKET socket)
{
	FD_SET(socket, &sockets_);
}

void SocketSet::ReserveRemove(const SOCKET socket)
{
	reserveDisconnectSockets_.emplace(socket);
}

void SocketSet::ExecuteRemove()
{
	for (auto socket : reserveDisconnectSockets_)
	{
		closesocket(socket);
		FD_CLR(socket, &sockets_);
	}
	reserveDisconnectSockets_.clear();
}
