#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE		1024
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE] = { 0, };
	int strLen = 0, recvLen = 0, recvCnt = 0;
	SOCKADDR_IN servAddr;

	if (argc != 3) {
		printf("Usage : %s <IP> <Port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect()");
	}
	else
	{
		printf("Connected..........\n");
	}


	while (1)
	{
		fputs("Input message (Q to q): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
		{
			break;
		}

		strLen = send(hSocket, message, strlen(message), 0);
		recvLen = 0;
		while (recvLen < strLen) 
		{
			recvCnt = recv(hSocket, &message[recvLen], BUF_SIZE - 1, 0);
			if (recvCnt == -1)
			{
				ErrorHandling("recv() error");
			}
			recvLen += recvCnt;
		}
		message[recvLen] = 0;
		printf("Message from server : %s", message);
	}
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message) 
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}