#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 100
void error_handling(const char* message);

int main(void) {
	int hFile = 0;
	char buf[BUF_SIZE] = { 0 };

	if (_sopen_s(&hFile, "data.txt", _O_RDONLY, _SH_DENYWR, _S_IREAD) != 0) {
		error_handling("open() error\n");
	}
	printf("file handle : %d\n", hFile);

	if (_read(hFile, buf, BUF_SIZE) == -1)
	{
		error_handling("read() error\n");
	}
	printf("file data : %s \n", buf);
	_close(hFile);
	return 0;
}


void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}