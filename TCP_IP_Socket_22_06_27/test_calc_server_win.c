#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE		1024
void ErrorHandling(const char* message);
int recvData(SOCKET hClntSock, char* message);
int msgToInt(const char* message, const int len);
int calculate(int* arr, int len, char op);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE] = { 0, };
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize = 0;
	int* arr;
	int result = 0;
	int opCount = 0;
	char optr;

	if (argc != 2) {
		printf("Usage : %s <Port> \n", argv[0]);
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
	memset(&clntAdr, 0, clntAdrSize);
	hClntSock = accept(hServSock, (PSOCKADDR)&clntAdr, &clntAdrSize);
	if (hClntSock == SOCKET_ERROR)
	{
		ErrorHandling("accept() error");
	}
	else
	{
		printf("Connected client %d \n", hClntSock);
	}

	opCount = recvData(hClntSock, message);
	arr = (int*)malloc(opCount * sizeof(int));
	if (arr == NULL)
	{
		ErrorHandling("malloc() error");
	}

	for (int i = 0; i < opCount; ++i)
	{
		arr[i] = recvData(hClntSock, message);
	}

	if (recv(hClntSock, &optr, 1, 0) == SOCKET_ERROR)
	{
		ErrorHandling("recv() operator error");
	}
	result = calculate(arr, opCount, optr);
	sprintf(message, "%d", result);
	send(hClntSock, message, strlen(message), 0);

	free(arr);
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

int recvData(SOCKET hClntSock, char* message)
{
	int strLen = 0;
	if ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) == 0)
	{
		ErrorHandling("recv() error");
	}
	return msgToInt(message, strLen);
}

int msgToInt(const char* message, const int len)
{
	int result = 0;
	for (int i = 0; i < len; ++i)
	{
		result *= 10;
		result += message[i] - '0';
	}
	return result;
}

int calculate(int* arr, int len, char op)
{
	int result = 0;
	switch (op)
	{
	case '+':
		for (int i = 0; i < len; ++i)
		{
			result += arr[i];
		}
		break;
	case '-':
		result += arr[0];
		for (int i = 1; i < len; ++i)
		{
			result -= arr[i];
		}
		break;
	case '*':
		result = 1;
		for (int i = 0; i < len; ++i)
		{
			result *= arr[i];
		}
		break;
	case '/':
		result = arr[0];
		for (int i = 1; i < len; ++i)
		{
			if (arr[i] == 0) {
				return -1;
			}
			result /= arr[i];
		}
		break;
	default:
		/* Nothing */
		break;
	}

	return result;
}
