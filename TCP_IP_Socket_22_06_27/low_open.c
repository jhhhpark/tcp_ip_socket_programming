#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

void error_handling(const char* message);

int main(void) {
	int hFile = 0;
	char buf[] = "Let's go!\n";

	if (_sopen_s(&hFile, "data.txt", _O_RDWR | _O_CREAT, _SH_DENYNO, _S_IREAD | _S_IWRITE) != 0) {
		error_handling("open() error! \n");
	}
	printf("file handle : %d \n", hFile);

	int i = 0, len = 0;
	while (buf[i] != '\0') { ++i; ++len; }
	if (_write(hFile, buf, sizeof(buf)) == -1) {
		error_handling("write() error! \n");
	}
	_close(hFile);
	return 0;
}

void error_handling(const char* message) 
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}