#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
#include <stdio.h>
#include "miniSocket.hpp"
#include <fstream>
#include <vector>
#pragma comment(lib, "ws2_32.lib")

int main() {

	// client side
	
	// set socket
	miniPlayer::miniClient myClient;

	// receive FMT
	miniPlayer::mini myPlayer = myClient.receiveFMT();

	// receive data
	myClient.receiveAndPlay(myPlayer);


	return 0;
}
