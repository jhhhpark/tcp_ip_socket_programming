#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE		1024
#define OPSZ			4
void ErrorHandling(const char* message);
int Calculate(int opnum, int opnds[], char oprator);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSocket, hClntSocket;
	char opinfo[BUF_SIZE];
	int result, opndCnt, i;
	int recvCnt, recvLen;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;

	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hServSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSocket, (PSOCKADDR)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}

	if (listen(hServSocket, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}

	clntAdrSz = sizeof(clntAdr);
	for (i = 0; i < 5; ++i)
	{
		opndCnt = 0;
		hClntSocket = accept(hServSocket, (PSOCKADDR)&clntAdr, &clntAdrSz);
		if (hClntSocket == SOCKET_ERROR)
		{
			ErrorHandling("accept() error");
		}
		recv(hClntSocket, (char*)&opndCnt, 1, 0);

		recvLen = 0;
		while ((opndCnt * OPSZ + 1) > recvLen)
		{
			recvCnt = recv(hClntSocket, &opinfo[recvLen], BUF_SIZE - 1, 0);
			recvLen += recvCnt;
		}
		result = Calculate(opndCnt, (int*)opinfo, opinfo[recvLen - 1]);
		send(hClntSocket, (char*)&result, sizeof(result), 0);
		closesocket(hClntSocket);
	}
	closesocket(hServSocket);
	WSACleanup();
	return 0;
}


void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int Calculate(int opnum, int opnds[], char oprator)
{
	int result = opnds[0], i;
	switch (oprator)
	{
	case '+':
		for (i = 1; i < opnum; ++i) result += opnds[i];
		break;
	case '-':
		for (i = 1; i < opnum; ++i) result -= opnds[i];
		break;
	case '*':
		for (i = 1; i < opnum; ++i) result *= opnds[i];
		break;
	default:
		/* Nothing */
		break;
	}
	return result;
}
