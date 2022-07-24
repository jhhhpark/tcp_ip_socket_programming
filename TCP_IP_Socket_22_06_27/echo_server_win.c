#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE];
	int strlen, i;

	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSize;

	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}

	clntAdrSize = sizeof(clntAddr);

	for (i = 0; i < 5; ++i)
	{
		hClntSock = accept(hServSock, (struct sockaddr*)&hClntSock, &clntAdrSize);
		if (hClntSock == -1)
		{
			ErrorHandling("accept() error");
		}
		else
		{
			printf("Connected client %d \n", hClntSock);
		}

		while ((strlen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
		{
			send(hClntSock, message, strlen, 0);
		}
		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}