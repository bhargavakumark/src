#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <sys/param.h>

#define BUFSIZE 104857600

ssize_t normal_read(char infd, int outfd, off_t offset, ssize_t size)
{
	ssize_t		read_size = 0, req_size = size, req_read_size;
	static char	buf[BUFSIZE];

	while (req_size > 0) {
		req_read_size = MIN(BUFSIZE, req_size);
		read_size = read(infd, buf, req_read_size);
		if (read_size < 0) {
			fprintf(stderr, "read error for fd %d : %s\n", infd,
							strerror(errno));
			break;
		}
		req_size -= read_size;
		if (req_read_size != read_size) { 
			fprintf(stderr, "partial read for fd %\n", infd);
			break;
		}
	}

	return size - req_size;
}

void usage(char *argv[])
{
	fprintf(stderr, "Usage : %s -f <input-file>\n", argv[0]);
}

int main (int argc, char *argv[])
{
	char		*filename = NULL, *outfilename = NULL;
	struct stat	fileStat;
	int		infd, outfd, err, opt;
	ssize_t		read_size;
	time_t		start_time, end_time;
	
	while ((opt = getopt(argc, argv,"i:o:" )) != EOF) {
		switch (opt) {
		case 'i':
			filename = optarg;
			break;
		case 'o':
			outfilename = optarg;
			break;
		default:
			usage(argv);
			exit(1);
		}
	}

	if (filename == NULL) {
		fprintf(stderr, "filename is not proivded with -f\n");
		usage(argv);
		exit(1);
	}
	if (outfilename == NULL)
		outfilename = "/dev/null";

	if (stat(filename, &fileStat) < 0) {
		fprintf(stderr, "stat error for %s: %s\n", filename, 
							strerror(errno));
		exit(1);
	}

	if ((fileStat.st_mode & S_IFMT) != S_IFREG) {
		fprintf(stderr, "not a regular file %s\n", filename);
		exit(1);
	}

	outfd = open("/dev/null", O_WRONLY|O_CREAT|O_TRUNC);
	if (outfd < 0) {
		fprintf(stderr, "open error for /dev/null %s\n", 
							strerror(errno));
		exit(1);
	}

	infd = open(filename, O_RDONLY);
	if (infd < 0) {
		fprintf(stderr, "open error for %s\n", filename, 
							strerror(errno));
		exit(1);
	}
	
	start_time = time(NULL);
	read_size = normal_read(infd, outfd, 0, fileStat.st_size);
	end_time = time(NULL);

	if (!err) {
		printf("Time taken : %lu\n", end_time - start_time);
		printf("Data Read : %lu\n", read_size);
		printf("Speed is : %f MB/sec\n", read_size * 1.0 / 
				(1024 * 1024) / (end_time - start_time));
	}

	return 0;
}
