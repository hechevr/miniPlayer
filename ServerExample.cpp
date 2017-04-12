#include <winsock.h>
#include <tchar.h>
#include <strsafe.h>
#include "miniSocket.hpp"
#include <fstream>
#include <vector>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[]) {

	// set socket
	miniPlayer::miniServer myServer;

	// receive
	char* buffer;
	buffer = myServer.miniReceiveMSG();
	printf("%s\n", buffer);

	
	// send data
	myServer.miniSendMSG("World");
	
	myServer.miniClose();

	return 0;
}
