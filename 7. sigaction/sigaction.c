#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig==SIGALRM)
		puts("Time out!");
	alarm(2);	
}

void keycontrol(int sig)
{
	if(sig == SIGINT)
		puts("CTRL+C pressed");
}

int main(int argc, char *argv[])
{
	int i;
	struct sigaction act, act2;
	act.sa_handler=timeout;
	act2.sa_handler=keycontrol;
	sigemptyset(&act.sa_mask);
	sigemptyset(&act2.sa_mask);
	act.sa_flags=0;
	act2.sa_flags=0;
	sigaction(SIGALRM, &act, 0);
	sigaction(SIGINT, &act2, 0);

	alarm(2);

	for(i=0; i<10; i++)
	{
		puts("wait...");
		sleep(100);
	}
	return 0;
}
