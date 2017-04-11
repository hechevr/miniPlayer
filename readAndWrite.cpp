#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define REC_BUFFER_SIZE 1000

int main() {
	WSADATA wsaData;
	SOCKET conn_sock;

	struct sockaddr_in remote_addr;

	WSAStartup(MAKEWORD(2, 1), &wsaData);

	conn_sock = socket(AF_INET, SOCK_STREAM, 0);

	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(8081);
	remote_addr.sin_addr.s_addr = inet_addr("192.168.110.11");


	connect(conn_sock, (struct sockaddr*)&remote_addr, sizeof(struct sockaddr));
	char *buffer = new char[sizeof(unsigned long)];
	unsigned long bytes = recv(conn_sock, buffer, sizeof(unsigned long), 0);
	// printf("size: %ul\n", bytes);
	for (int i = 0; i < sizeof(unsigned long); i++) {
		printf("%d\n", buffer[i]);
	}
	unsigned long size;
	size = *((unsigned long*)buffer);
	printf("size: %ul\n", size);
	buffer = new char[size];
	unsigned long i;
	char *bu = new char[REC_BUFFER_SIZE];
	for (i = 0; i < size; i += REC_BUFFER_SIZE) {
		bytes = recv(conn_sock, bu, sizeof(char)*REC_BUFFER_SIZE, 0);
		for (int j = 0; j < REC_BUFFER_SIZE; j++) {
			buffer[i + j] = bu[j];
			// printf("%d\n", buffer[i + j]);
		}
	}
	printf("total: %ul\n", i);
	FILE *file;
	file = fopen("test.mp3", "wb");
	fwrite(buffer, sizeof(char), size, file);
	fclose(file);
	return 0;
}
