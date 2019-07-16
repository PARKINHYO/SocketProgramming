#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char *argv[])
{
	struct iovec vec[3];
	char buf1[]="ABCDEFG";
	char buf2[]="1234567";
	char buf3[] = "INHYOPARK";
	int str_len;

	vec[0].iov_base=buf1;
	vec[0].iov_len=3;
	vec[1].iov_base=buf2;
	vec[1].iov_len=4;
	vec[2].iov_base=buf3;
	vec[2].iov_len=5;
	
	str_len=writev(1, vec, 3);
	puts("");
	printf("Write bytes: %d \n", str_len);
	return 0;
}
