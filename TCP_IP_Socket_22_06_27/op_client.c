#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE			1024
#define RLT_SIZE			4
#define OPSZ				4
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char opmsg[BUF_SIZE];
	SOCKADDR_IN servAdr;
	int result, opndCnt, i;
	if (argc != 3)
	{
		printf("Usage : %s <ip> <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (PSOCKADDR)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error");
	}
	else
	{
		printf("Connected.......... \n");
	}

	fputs("Operand count: ", stdout);
	scanf_s("%d", &opndCnt);
	opmsg[0] = (char)opndCnt;

	for (i = 0; i < opndCnt; ++i)
	{
		printf("Operand %d: ", i + 1);
		scanf_s("%d", (int*)&opmsg[i * OPSZ + 1]);
	}
	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf_s("%c", &opmsg[opndCnt * OPSZ + 1], 1);
	send(hSocket, opmsg, opndCnt * OPSZ + 2, 0);
	recv(hSocket, (char*)&result, RLT_SIZE, 0);

	printf("Operation result: %d \n", result);

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