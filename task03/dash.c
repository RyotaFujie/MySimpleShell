
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>

#define BUFSIZE  1024
#define ARGVSIZE 100

const char whitespace[] = " \t\r\n\v";

int parsecmd(char **argv, int *len, char *buf, char *ebuf)
{
	char *s;
	int  i = 0;
	
	s = buf;

	while (s < ebuf) {
		while (s < ebuf && strchr(whitespace, *s)) s++;
		if (ebuf <= s) return -1;

		argv[i++] = s;
		while (s < ebuf && !strchr(whitespace, *s)) s++;
		*s = '\0'; 
		s++;
	}

	//返り値
	*len = i;
	return 1;
}

void runcmd(char *buf)
{
	char *argv[ARGVSIZE];
	int len;
		
	memset(argv, 0, ARGVSIZE);
	if (parsecmd(argv, &len, buf, &buf[strlen(buf)]) > 0)
		if (execvp(argv[0], argv) < 0)
			perror("execvp");		
	exit(-1);
}

int getcmd(char *buf, int len)
{
	printf("> ");
	fflush(stdin);
	memset(buf, 0, len);
	fgets(buf, len, stdin);

	if (buf[0] == 0)
		return -1;
	return 0;
}

int main(int argc, char**argv)
{
	static char buf[BUFSIZE];

	while(getcmd(buf, BUFSIZE) >= 0) {
		if (fork() == 0)
			runcmd(buf);
		wait(NULL);
	}

	exit(0);
}
