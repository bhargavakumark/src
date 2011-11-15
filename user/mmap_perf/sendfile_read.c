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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/***********************************************************
 *
 * To use this program you would need a TTCP or similar program
 * which provides a socket to which we can connect to and discards
 * the data that we send
 *
 * If using ttcp then, run TTCP as 
 * ./ttcp -p 2000 -r
 * 
 ************************************************************/

#define BUFSIZE 104857600

ssize_t sendfile_read(char infd, int outfd, off_t offset, ssize_t size)
{
	ssize_t		read_size = 0, req_size = size, req_read_size;
	static char	buf[BUFSIZE];

	while (req_size > 0) {
		req_read_size = MIN(BUFSIZE, req_size);
		read_size = sendfile(outfd, infd, 0, req_read_size);
		if (read_size < 0) {
			fprintf(stderr, "read error for fd %d : %s\n", infd,
							strerror(errno));
			break;
		}
		req_size -= read_size;
		if (req_read_size != read_size) { 
			fprintf(stderr, "partial read for fd %d\n", infd);
			break;
		}
	}

	return size - req_size;
}

void usage(char *argv[])
{
	fprintf(stderr, "Usage : %s -i <input-file>\n", argv[0]);
}

int main (int argc, char *argv[])
{
	char		*filename = NULL, *outfilename = NULL;
	struct stat	fileStat;
	int		infd, outfd, err, opt;
	ssize_t		read_size;
	time_t		start_time, end_time;
	in_addr_t	saddr;

	struct sockaddr_in	sin1;
	
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
#if 0
	outfd = open("/dev/null", O_WRONLY|O_CREAT|O_TRUNC);
	if (outfd < 0) {
		fprintf(stderr, "open error for /dev/null %s\n", 
							strerror(errno));
		exit(1);
	}
#endif
	outfd = socket(AF_INET, SOCK_STREAM, 0);
	if (outfd < 0) {
		fprintf(stderr, "open error for socket() %s\n", 
							strerror(errno));
		exit(1);
	}
	if (!inet_pton(AF_INET, "127.0.0.1", &saddr)) {
		fprintf(stderr, "IP address 127.0.0.1 is not valid %s\n",
							strerror(errno));
		exit(1);
	}

	infd = open(filename, O_RDONLY);
	if (infd < 0) {
		fprintf(stderr, "open error for %s\n", filename, 
							strerror(errno));
		exit(1);
	}
	
	memset(&sin1, 0, sizeof(sin1));
	sin1.sin_family      = AF_INET;
	sin1.sin_addr.s_addr = saddr;
	sin1.sin_port        = htons(2000);

	if (connect(outfd, (struct sockaddr *)&sin1, sizeof(sin1)) < 0) {
		fprintf(stderr, "connect error: %s\n", strerror(errno));
		exit(1);
	}

	start_time = time(NULL);
	read_size = sendfile_read(infd, outfd, 0, fileStat.st_size);
	end_time = time(NULL);

	if (!err) {
		printf("Time taken : %lu\n", end_time - start_time);
		printf("Data Read : %lu\n", read_size);
		printf("Speed is : %f MB/sec\n", read_size * 1.0 / 
				(1024 * 1024) / (end_time - start_time));
	}

	return 0;
}
