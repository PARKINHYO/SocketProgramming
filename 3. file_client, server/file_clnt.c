#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int clnt_sd;
	FILE *fp;
	int i;

	char buf[BUF_SIZE];
	char file_name[BUF_SIZE];
	char file_name2[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_adr;
	if(argc!=3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	for(i=0; i<5; i++)
	{
		clnt_sd=socket(PF_INET, SOCK_STREAM, 0);
		if(clnt_sd == -1)
			error_handling("socket() error");

		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family=AF_INET;
		serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
		serv_adr.sin_port=htons(atoi(argv[2]));

		if(connect(clnt_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
			error_handling("connect() error!"); 
		else
			puts("Connected.........");

		printf("전송받을 파일 이름: ");
		scanf("%s", file_name);

		printf("전송을 받아 저장할 파일 이름: ");
		scanf("%s", file_name2);

		fp = fopen(file_name2, "wt");
		if(fp == NULL)
			error_handling("FIle open error");

		write(clnt_sd, file_name, strlen(file_name)+1);	

		while((read_cnt=read(clnt_sd, buf, BUF_SIZE))!=0)
			fwrite((void*)buf, 1, read_cnt, fp);

		fclose(fp);
		close(clnt_sd);
	}

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
