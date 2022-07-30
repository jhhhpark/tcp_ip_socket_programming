#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define NAME_SIZE			256
#define BUF_SIZE			1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;
	char buf[BUF_SIZE], fileName[NAME_SIZE], newFileName[NAME_SIZE] = "new_";
	int recvLen;

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
		printf("Connect.............. \n");
	}

	fputs("want file name : ", stdout);
	scanf_s("%s", fileName, sizeof(fileName));
	fileName[strlen(fileName)] = 0;
	send(hSocket, fileName, strlen(fileName)+1, 0);

	strcat_s(newFileName, NAME_SIZE, fileName);
	FILE* stream;
	errno_t err = fopen_s(&stream, newFileName, "wt");
	if (err != 0)
	{
		ErrorHandling("wrong create file error");
	}
	else
	{
		while ((recvLen = recv(hSocket, buf, BUF_SIZE-1, 0)) != 0)
		{
			fwrite(buf, 1, recvLen, stream);
		}
		printf("SUCCESS!! : create file %s. \n", newFileName);
		fclose(stream);
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