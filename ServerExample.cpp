#include <winsock.h>
#include <tchar.h>
#include <strsafe.h>
#include "miniSocket.hpp"
#include <fstream>
#include <vector>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cout << "Usage: Server.exe [filename]" << std::endl;
		return 0;
	}

	// set player
	miniPlayer::mini myPlayer(argv[1]);

	// set socket
	miniPlayer::miniServer myServer;

	// send data
	myServer.miniSend(myPlayer);

	

	return 0;
}
