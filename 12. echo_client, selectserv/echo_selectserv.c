#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	FILE* fp;
	char buf[BUF_SIZE];
	char buf_serv[BUF_SIZE];
	char tmp[BUF_SIZE];
	char file_name[BUF_SIZE][BUF_SIZE]; // 파일이름을 클라이언트마다 따로 저장을 위해 2차원 배열 생성
	char finish[] = "End of Message";
	char txt[] = ".txt"; 
	int t[BUF_SIZE]={0, }; // t배열 생성 후 모두 0으로 초기화

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(0, &reads); // 0 set
	FD_SET(serv_sock, &reads);
	fd_max=serv_sock;

	while(1)
	{
		cpy_reads=reads;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;

		if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1)
			break;

		if(fd_num==0)
			continue;

		for(i=0; i<fd_max+1; i++)
		{
			if(FD_ISSET(i, &cpy_reads))
			{
				if(i==0) // 서버에서 입력시 서버에 출력하는 코드
				{
					fgets(buf_serv, BUF_SIZE, stdin);
					printf("Message from server: %s", buf_serv);
				}
				else if(i==serv_sock)     // connection request!
				{
					adr_sz=sizeof(clnt_adr);
					clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if(fd_max<clnt_sock)
						fd_max=clnt_sock;
					printf("connected client: %d \n", clnt_sock);
				}
				else    // read message!
				{
					if(t[i]==0)//파일 이름을 전송받는 것은 클라이언트당 한번만 진행하기 위해 조건을 주고 벗어나게끔 만든다.
					{
						str_len = read(i, file_name[i], BUF_SIZE);
						write(i, file_name[i], str_len);
						file_name[i][str_len] = 0;
						sprintf(tmp, "%d", i);//파일 디스크립터(int)를 문자열로 변환
						strcat(file_name[i], tmp);
						strcat(file_name[i], txt);//입력받은값 + 파일디스크립터 + .txt형태로 문자열생성
						t[i]++;
					}
					str_len=read(i, buf, BUF_SIZE);
					if(str_len==0)    // close request!
					{
						FD_CLR(i, &reads);
						close(i);
						t[i]--;
						printf("closed client: %d \n", i);
					}
					else
					{
						fp = fopen(file_name[i], "at+");//파일오픈
						buf[str_len] = 0;
						if (!strcmp(buf, "Q\n") || !strcmp(buf, "q\n")) //Q나 q가 클라이언트로부터 오면
						{
							write(i, finish, strlen(finish));//End of Message전송
							fwrite((void*)buf, 1, str_len, fp);
							fclose(fp);
							break;
						}
						fwrite((void*)buf, 1, str_len, fp);
						write(i, buf, str_len);    // echo!
						fclose(fp);//파일 닫기
					}
				}

			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
