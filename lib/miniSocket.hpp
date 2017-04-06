#include <winsock.h>
#include <fstream>
#include "playWave.hpp"

namespace miniPlayer
{
	class miniServer
	{
	public:

		// constructor
		miniServer();

		// close
		void miniClose();

		// WSADATA
		WSADATA wsaData;

		// sock
		SOCKET listen_sock;
		SOCKET new_sock;

		// addr
		struct sockaddr_in my_addr;

		// build Server
		void miniSend(mini &player);

	};

	class miniClient
	{
	public:
		// constructor
		miniClient(char* address = "192.168.110.46");

		// close
		void miniClose();

		// WSADATA
		WSADATA wsaData;

		// sock
		SOCKET conn_sock;
		
		// addr
		struct sockaddr_in remote_addr;

		// build Client
		void miniReceive();

	};

}