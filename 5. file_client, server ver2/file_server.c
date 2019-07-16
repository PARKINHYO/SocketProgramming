#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sd, clnt_sd;
	FILE * fp;
	char buf[BUF_SIZE];
	int read_cnt;
	char file_name[BUF_SIZE];
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sd=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sd == -1)
		error_handling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sd, 5) == -1)
		error_handling("listen() error");

	clnt_adr_sz=sizeof(clnt_adr);
	while(1)
	{	    
		clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		read(clnt_sd, file_name, BUF_SIZE);
		if(!strcmp(file_name, "tmp"))
			continue;
		fp = fopen(file_name, "wt");

		while((read_cnt = read(clnt_sd, buf, BUF_SIZE)) != 0){
			fwrite((void*)buf, 1, read_cnt, fp);
		}

		puts("Received file data");
		write(clnt_sd, "Thank you", 10);
		exit(1);

		fclose(fp);
		close(clnt_sd);
	}

	close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
