/*
* 내용확인문제 06번
*/

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")


void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30] = { 0 };
	int strLen = 0;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!");
	}

	// busy waiting
	for (int i = 0; i < 3000; ++i)
	{
		printf("Wait time %d \n", i);
	}
	strLen = recv(hSocket, message, sizeof(message), 0);
	if (strLen == SOCKET_ERROR)
	{
		ErrorHandling("recv() error!");
	}
	printf("Message from server: %s \n", message);
	printf("Function read call count: %d \n", strLen);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}