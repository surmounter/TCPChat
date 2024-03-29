#pragma once
#include <set>
#include <winsock2.h>
#include <xutility>


class SocketSet
{
public:
	SocketSet();
	fd_set Get() const { return sockets_; }
	void Add(const SOCKET socket);
	void ReserveRemove(const SOCKET socket);
	void ExecuteRemove();
	template<typename Functor>
	void ForeachSockets(Functor&& functor) const
	{
		for (size_t i = 0; i < sockets_.fd_count; ++i)
		{
			functor(sockets_.fd_array[i]);
		}
	}
	template<typename Functor>
	void ForeachHandleableSockets(const fd_set handleableSockets, Functor&& functor) const
	{
		for (size_t i = 0; i < sockets_.fd_count; ++i)
		{
			auto socket = sockets_.fd_array[i];
			if (FD_ISSET(socket, &handleableSockets) == false)
			{
				continue;
			}
			functor(socket);
		}
	}
private:
	fd_set sockets_;
	std::set<SOCKET> reserveDisconnectSockets_;
};