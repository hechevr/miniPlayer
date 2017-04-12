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
	miniPlayer::miniClient myClient("192.168.110.78");

	// send
	myClient.miniSendMSG("Hello");
	printf("send: ");

	
	// receive
	char *message = new char[50];
	message = myClient.miniReceiveMSG();
	printf("%s\n", message);
	
	myClient.miniClose();

	return 0;
}
