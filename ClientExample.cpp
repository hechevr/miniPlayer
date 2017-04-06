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

	// receive and play data
	myClient.miniReceive();


	return 0;
}
