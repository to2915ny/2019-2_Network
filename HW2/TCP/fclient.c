#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <string.h>
#define BUFSIZE 1024
//TCPclient

void error_handling(char *message);

int main(int argc,char **argv)
{
	int sock;
	char message[BUFSIZE]={0};
	int str_len;
	struct sockaddr_in serv_addr;
	FILE *fp;

	if(argc!=4){
		printf("Usage : %s <IP> <port> <file_name>\n",argv[0]);
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
	send(sock,argv[3],BUFSIZE,0);
	fp=fopen(argv[3],"rb");

	while(1)
	{
		str_len = fread(message,sizeof(char),BUFSIZE,fp);
		if(str_len<BUFSIZE){

			send(sock,message,str_len,0);
			printf("Client : %d\n",str_len);
			break;

		}

		send(sock,message,str_len,0);
		printf("Client : %d\n",str_len);	
	}

	fclose(fp);
	printf("File sent!\n");

	close(sock);

	return 0;

}
void error_handling(char *message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
