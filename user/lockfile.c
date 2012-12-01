#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

extern char *optarg;

static char *usage =
"usage: lockfile [-bw] [-s secs] [-u] [-f file_name]\n";

static int blocking = 0;
static int sleep_time = 10;
static char  *file_name = "testfile";
static int unlock = 0;
static int write_lock = 0;
static pid_t my_pid;

main (int argc, char *argv[])
{

	int fd;
	int open_flag;
	int opt;
	struct flock linfo;

	my_pid = getpid();
	while ((opt = getopt(argc, argv,"bs:f:uw" )) != EOF) {

		switch (opt) {
		case 'b':
			blocking = 1;
			break;
		case 's':
			sleep_time = atoi(optarg);
			unlock = 1;
			break;
		case 'f':
			file_name = optarg;
			break;
		case 'u':
			unlock = 1;
			break;
		case 'w':
			write_lock = 1;
			break;
		default:
			fprintf(stderr, usage);
			exit (1);
		}
	}

	open_flag = (write_lock ? O_RDWR : O_RDONLY);

	if ((fd = open(file_name, open_flag, 0666)) < 0) {
		if (errno == ENOENT) {
			open_flag |= O_CREAT;
			fd = open(file_name, open_flag, 0666);
		}
		if (fd < 0) {
			fprintf(stderr, "can't open file %s, error = %s\n",
				file_name, strerror(errno));
			exit (1);
		}
	}

	linfo.l_type = (write_lock ? F_WRLCK : F_RDLCK);
	linfo.l_whence = SEEK_SET;
	linfo.l_start = 0;
	linfo.l_len = 0;

	if (fcntl(fd, blocking ? F_SETLKW : F_SETLK, &linfo) < 0 ) {
		fprintf(stderr, "%d: can't set %s lock on %s : %s\n",
			my_pid, write_lock ? "exclusive" : "shared", file_name,
			strerror(errno));
		exit(1);
	}
	printf("%d: got %s lock, sleeping ...\n", my_pid,
		write_lock ? "exclusive" : "shared");
	fflush(stdout);

	if (sleep_time)
		sleep(sleep_time);

	if (unlock) {
		printf("%d: unlocking ...\n", my_pid);
		fflush(stdout);
		linfo.l_type = F_UNLCK;
		linfo.l_whence = SEEK_SET;
		linfo.l_start = 0;
		linfo.l_len = 0;

		if (fcntl(fd, F_SETLKW, &linfo) < 0) {
			fprintf(stderr, "%d: can't unlock %s : %s\n",
			my_pid, file_name, strerror(errno));
			exit(1);
		}
		printf("%d: unlock done ...\n", my_pid);
		fflush(stdout);
	}

	sleep(4);
	printf("%d: done ...\n", my_pid);
	fflush(stdout);
	return (0);
}
