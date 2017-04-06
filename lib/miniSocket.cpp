#include <winsock.h>
#include <fstream>
#include <iostream>
#include "miniSocket.hpp"

#pragma comment(lib, "ws2_32.lib")

#define MY_PORT 8081
#define SEND_BUFFER_SIZE 8000
#define REC_BUFFER_SIZE 8000


namespace miniPlayer
{
	miniServer::miniServer() {

		int err = WSAStartup(MAKEWORD(2, 1), &wsaData);
		if (err) {
			std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
			exit(-1);
		}

		std::cout << "WSAStartup" << std::endl;

		listen_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_sock == INVALID_SOCKET) {
			std::cerr << "Invalid socket: " << WSAGetLastError() << std::endl;
			WSACleanup();
			exit(-1);
		}
		std::cout << "Set socket" << std::endl;

		my_addr.sin_family = AF_INET;
		my_addr.sin_port = htons(MY_PORT);
		my_addr.sin_addr.s_addr = INADDR_ANY;

		err = bind(listen_sock, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
		if (err == SOCKET_ERROR) {
			std::cerr << "Bind error: " << WSAGetLastError() << std::endl;
			WSACleanup();
			exit(-1);
		}
		std::cout << "Bind successfully" << std::endl;

		std::cout << "Wait for connection" << std::endl;

	}

	void miniServer::miniClose() {

		closesocket(new_sock);
		closesocket(listen_sock);
		WSACleanup();

	}

	void miniServer::miniSend(mini &player) {

		listen(listen_sock, SOMAXCONN);
		new_sock = accept(listen_sock, NULL, NULL);

		// send RIFFInfo
		char* buffer = new char[sizeof(MMCKINFO)];
		memcpy(buffer, &player.RIFFInfo, sizeof(MMCKINFO));
		send(new_sock, (char*)buffer, sizeof(MMCKINFO), 0);

		// send FMTChunkInfo
		memset(buffer, 0, sizeof(MMCKINFO));
		memcpy(buffer, &player.FMTChunkInfo, sizeof(MMCKINFO));
		send(new_sock, (char*)buffer, sizeof(MMCKINFO), 0);

		// send DATAChunkInfo
		memset(buffer, 0, sizeof(MMCKINFO));
		memcpy(buffer, &player.DATAChunkInfo, sizeof(MMCKINFO));
		send(new_sock, (char*)buffer, sizeof(MMCKINFO), 0);

		// send waveFmt
		buffer = new char[sizeof(WAVEFORMATEX)];
		memcpy(buffer, &player.waveFmt, sizeof(WAVEFORMATEX));
		send(new_sock, (char*)buffer, sizeof(WAVEFORMATEX), 0);

		// send data
		buffer = new char[SEND_BUFFER_SIZE];
		printf("%ul %ul\n", strlen(buffer), SEND_BUFFER_SIZE);
		memset(buffer, 0, SEND_BUFFER_SIZE);
		printf("data size: %ul send_buffer_size: %ul\n", player.dataSize, SEND_BUFFER_SIZE);

		unsigned long index = 0;
		for (index = 0; index < player.dataSize; index += SEND_BUFFER_SIZE) {
			for (int i = 0; i < SEND_BUFFER_SIZE; i++) {
				buffer[i] = player.data[i + index];
				if (i + index >= player.dataSize - 1) {
					break;
				}
			}
			send(new_sock, (char*)buffer, sizeof(char)*SEND_BUFFER_SIZE, 0);
			printf("send %ul\n", sizeof(char)*SEND_BUFFER_SIZE);
		}

		miniClose();
	}

	miniClient::miniClient(char* address) {
		int err = WSAStartup(MAKEWORD(2, 1), &wsaData);
		if (err) {
			std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
			exit(-1);
		}

		std::cout << "WSAStartup" << std::endl;

		conn_sock = socket(AF_INET, SOCK_STREAM, 0);

		if (conn_sock == INVALID_SOCKET) {
			std::cerr << "Invalid socket: " << WSAGetLastError() << std::endl;
			WSACleanup();
			exit(-1);
		}
		std::cout << "Set socket" << std::endl;

		remote_addr.sin_family = AF_INET;
		remote_addr.sin_port = htons(MY_PORT);
		remote_addr.sin_addr.s_addr = inet_addr(address);

		std::cout << "Try to connect: " << address << std::endl;

		connect(conn_sock, (struct sockaddr*)&remote_addr, sizeof(struct sockaddr));

		std::cout << "Connected. Start to copy file" << std::endl;
	}

	void miniClient::miniClose() {

		closesocket(conn_sock);
		WSACleanup();

	}
	void miniClient::miniReceive() {

		// receive RIFFInfo
		char* buffer = new char[sizeof(MMCKINFO)];
		unsigned int bytes_recvd = recv(conn_sock, buffer, sizeof(MMCKINFO), 0);
		MMCKINFO RI;
		memcpy(&RI, buffer, sizeof(MMCKINFO));
		printf("Received RIFFInfo: %d bytes\n", bytes_recvd);

		// receive FMTChunkInfo
		memset(buffer, 0, sizeof(MMCKINFO));
		bytes_recvd = recv(conn_sock, buffer, sizeof(MMCKINFO), 0);
		MMCKINFO FI;
		memcpy(&FI, buffer, sizeof(MMCKINFO));
		printf("Received FMTChunkInfo: %d bytes\n", bytes_recvd);

		// receive DATAChunkInfo
		memset(buffer, 0, sizeof(MMCKINFO));
		bytes_recvd = recv(conn_sock, buffer, sizeof(MMCKINFO), 0);
		MMCKINFO DI;
		memcpy(&DI, buffer, sizeof(MMCKINFO));
		printf("Received DATAChunkInfo: %d bytes\n", bytes_recvd);

		// receive waveFMT
		buffer = new char[sizeof(WAVEFORMATEX)];
		memset(buffer, 0, sizeof(WAVEFORMATEX));
		bytes_recvd = recv(conn_sock, buffer, sizeof(WAVEFORMATEX), 0);
		WAVEFORMATEX WF;
		memcpy(&WF, buffer, sizeof(WAVEFORMATEX));
		printf("Received waveFMT: %d bytes\n", bytes_recvd);

		// create myPlayer using RI, FI, DI, WF
		miniPlayer::mini myPlayer(RI, FI, DI, WF);

		// receive data
		buffer = new char[REC_BUFFER_SIZE];
		printf("size: %ul\n", myPlayer.dataSize);

		myPlayer.data = new char[myPlayer.dataSize];
		unsigned long index;
		for (index = 0; index < myPlayer.dataSize; index += REC_BUFFER_SIZE) {
			bytes_recvd = recv(conn_sock, (char*)buffer, sizeof(char)*REC_BUFFER_SIZE, 0);
			printf("received data: %ul\n", bytes_recvd);
			for (int i = 0; i < REC_BUFFER_SIZE; i++){
				myPlayer.data[i + index] = buffer[i];
				if (i + index >= myPlayer.dataSize - 1) {
					break;
				}
			}
		}

		miniClose();
		myPlayer.miniPlay();
	}

}