#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	/* inet_addr 함수 호출 예 */
	const char* addr = "127.212.124.78";

	// inet_addr() -> deprecated
	//unsigned long conv_addr = inet_addr(addr);
	//if (conv_addr == INADDR_NONE)
	//{
	//	prinitf("Error occured! \n");
	//}
	//else
	//{
	//	printf("Network ordered integer addr: %#lx \n", conv_addr);
	//}

	unsigned long conv_addr;
	if (inet_pton(AF_INET, addr, &conv_addr) == -1)
	{
		printf("Error occured! \n");
	}
	else
	{
		printf("Network ordered integer addr: %#lx \n", conv_addr);
	}

	/* inet_ntoa 함수의 호출 예 */
	{
		struct sockaddr_in addr;
		char* strPtr;
		char strArr[20];

		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = htonl(0x1020304);

		// inet_ntoa() -> deprecated
		//strPtr = inet_ntoa(addr.sin_addr); 
		//strcpy_s(strArr, sizeof(strArr), strPtr);
		inet_ntop(AF_INET, &addr.sin_addr, strArr, sizeof(strArr));
		printf("Dotted-Decimal notation3 %s \n", strArr);
	}

	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message) 
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}