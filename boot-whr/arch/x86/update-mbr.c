#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE 446

int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "Usage: %s <mbr> <dev>\n", argv[0]);
		return -1;
	}

	char buffer[BUF_SIZE];
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror(argv[1]);
		return 1;
	}
	if(read(fd, buffer, BUF_SIZE) != BUF_SIZE) {
		perror("read");
		return 2;
	}
	close(fd);

	fd = open(argv[2], O_WRONLY);
	if(fd == -1) {
		perror(argv[2]);
		return 1;
	}
	if(write(fd, buffer, BUF_SIZE) != BUF_SIZE) {
		perror("write");
		return 2;
	}
	close(fd);

	return 0;
}
