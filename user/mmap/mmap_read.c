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

ssize_t mmap_read(char infd, int outfd, off_t offset, ssize_t size)
{
	caddr_t		addr;
	ssize_t		read_size = 0, req_size = size;
	off_t		page_seek, page_offset, map_size, cur_offset;

	static char	buf[BUFSIZE];

	page_seek = offset & (PAGE_SIZE - 1);
	page_offset = offset - page_seek;
	map_size = req_size + page_seek;

	addr = mmap(0, map_size, PROT_READ, MAP_SHARED, infd, page_offset);
	if (addr == MAP_FAILED) {
		fprintf(stderr, "mmap failed :%s\n", strerror(errno));
		return -1;
	}
	
//	write_size = write(outfd, ((char *)addr) + page_seek, req_size);

	cur_offset = page_seek;
	while (req_size > 0) {
		read_size = MIN(BUFSIZE, req_size);
		memcpy(buf, ((char *)addr) + cur_offset, read_size);
		req_size -= read_size;
		cur_offset += read_size;
	}
	munmap(addr, map_size);

	return 0;
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
	err = mmap_read(infd, outfd, 0, fileStat.st_size);
	end_time = time(NULL);

	if (!err) {
		printf("Time taken : %lu sec\n", end_time - start_time);
		printf("Data Read : %lu bytes\n", fileStat.st_size);
		printf("Speed is : %f MB/sec\n", fileStat.st_size * 1.0 / 
				(1024 * 1024) / (end_time - start_time));
	}

	return 0;
}
