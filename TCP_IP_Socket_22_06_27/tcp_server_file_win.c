#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define NAME_SIZE		256
#define BUF_SIZE		1024
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;

	FILE* stream;
	char fileName[NAME_SIZE], buf[BUF_SIZE];
	int readLen;

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

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));
	if (bind(hServSock, (PSOCKADDR)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}

	clntAdrSize = sizeof(clntAdr);
	hClntSock = accept(hServSock, (PSOCKADDR)&clntAdr, &clntAdrSize);
	if (hClntSock == INVALID_SOCKET)
	{
		ErrorHandling("accept() error");
	}
	else
	{
		printf("Connected from client : %d \n", hClntSock);
	}

	recv(hClntSock, fileName, NAME_SIZE-1, 0);
	errno_t err = fopen_s(&stream, fileName, "r");
	if (err != 0)
	{
		ErrorHandling("file not found");
	}
	else
	{
		while ((readLen = fread_s((void*)buf, BUF_SIZE, 1, BUF_SIZE, stream)) != 0)
		{
			send(hClntSock, buf, readLen, 0);
		}
		fclose(stream);
	}


	closesocket(hClntSock);
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