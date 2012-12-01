#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int ac, char *av[])
{
	int f = open(av[1], O_WRONLY|O_TRUNC|O_CREAT, 644);
	char buf[10240];
	int i;
	int pid = 0;
	int status;

	if (f < 0) {
		perror(av[1]);
		return 1;
	}

	pid = fork();

	for (i=0; i < 1000; i++) {
		memset(buf, i, sizeof(buf));
		if (write(f, buf, sizeof(buf)) != sizeof(buf)) {
			perror("write failed");
			return 1;
		}
	}
	if (pid) 
		waitpid(pid, &status, 0);

	close(f);
	return 0;
}

