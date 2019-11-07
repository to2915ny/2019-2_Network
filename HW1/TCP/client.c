#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <string.h>

#define BUFSIZE 1024

void error_handling(char *message);

int main(int argc,char **argv)
{
	int sock;
	char message[BUFSIZE];
	int str_len;
	struct sockaddr_in serv_addr;

	if(argc!=3){
		printf("Usage : %s <IP><port>\n",argv[0]);
		exit(1);
	}
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock == -1)
		error_handling("socket() error");
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("connect() error");
	
	char MSG1[] = "0123456789\n";
	char MSG2[] = "ABCDEFGHIJ\n";
	char MSG3[] = "KLMNOPQRST\n";
	send(sock,MSG1,strlen(MSG1),0);
	send(sock,MSG2,strlen(MSG2),0);
	send(sock,MSG3,strlen(MSG3),0);
	while(1){
		fputs("Input message to send (q to quit) : ",stdout);
		fgets(message,BUFSIZE,stdin);
		if(!strcmp(message,"q\n")) break;
		send(sock,message,strlen(message),0);

	}
	close(sock);
	return 0;
}
void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
