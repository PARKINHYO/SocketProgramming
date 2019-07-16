#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

void error_handling(char *message);

int main(int argc, char *argv[]) 
{
	int tcp_sock, udp_sock;
	int sock_type;
	int opt_val;
	socklen_t optlen;
	int state;
	
	optlen=sizeof(opt_val);
	tcp_sock=socket(PF_INET, SOCK_STREAM, 0);
	//udp_sock=socket(PF_INET, SOCK_DGRAM, 0);	
	//printf("SOCK_STREAM: %d \n", SOCK_STREAM);
	//printf("SOCK_DGRAM: %d \n", SOCK_DGRAM);
	
	state=getsockopt(tcp_sock, IPPROTO_TCP, TCP_NODELAY,  (void*)&opt_val, &optlen);
	if(state)
		error_handling("getsockopt() error!");
	if(opt_val == 0)
		printf("Nagle 알고리즘 ON\n\n");


	opt_val = 1;
	state = setsockopt(tcp_sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, sizeof(opt_val));
	if(state)
		error_handling("setsockopt() error!");
	if(opt_val == 1)
		printf("Nagle 알고리즘 OFF\n");
	
	//state=getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&opt_val, &optlen);
	//if(state)
	//	error_handling("getsockopt() error!");
	//printf("Socket type two: %d \n", sock_type);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
