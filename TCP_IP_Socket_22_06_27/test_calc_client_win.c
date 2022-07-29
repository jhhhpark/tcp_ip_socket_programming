#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE			1024
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	char message[BUF_SIZE];
	SOCKET hSocket;
	SOCKADDR_IN servAdr;
	int opCount = 0;
	int operand = 0;
	char optr;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port> \n", argv[0]);
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
		printf("Connected...........\n");
	}

	printf("Operand count: ");
	scanf_s("%d", &opCount);
	sprintf(message, "%d", opCount);
	send(hSocket, message, strlen(message), 0);

	for (int i = 0; i < opCount; ++i)
	{
		printf("Operand %d : ", i + 1);
		scanf_s("%d", &operand);
		sprintf(message, "%d", operand);
		send(hSocket, message, strlen(message), 0);
	}

	printf("Operator : ");
	while (getchar() != '\n') { }
	optr = getchar();
	send(hSocket, &optr, sizeof(optr), 0);

	if (recv(hSocket, message, BUF_SIZE, 0) != 0)
	{
		ErrorHandling("recv() error");
	}

	printf("Operation result: %d", atoi(message));
	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}