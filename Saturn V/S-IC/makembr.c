#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if(argc != 3) {
		printf("Usage: %s <bootsector> <dev>\n", argv[0]);
		return 1;
	}	
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror("read error");
		return 1;
	}
	char buf[446];
	read(fd, buf, 446);
	close(fd);
	fd = open(argv[2], O_WRONLY);
	if(fd == -1) {
		perror("read error");
		return 1;
	}

	write(fd, buf, 446);
	return 0;
}

