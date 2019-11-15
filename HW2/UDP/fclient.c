#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <string.h>
#include <fcntl.h>
//UDPclient
#define BUFSIZE 4096
void error_handling(char *message);
int main(int argc, char **argv){
	int sock;
	char message[BUFSIZE]={0};
	int str_len, addr_size, i;
	struct sockaddr_in serv_addr;
	struct sockaddr_in from_addr;
	FILE *fp;
	if(argc!=4){
		printf("Usage : %s <IP> <port> <filename>\n", argv[0]);
		exit(1);
	}   
	sock=socket(PF_INET, SOCK_DGRAM, 0); 
	if(sock == -1) 
		error_handling("UDP socket not made");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	int flags = fcntl(sock,F_GETFL,0);//change it to non-blocking
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	while(1){

		sendto(sock, argv[3], strlen(argv[3]), 0, (struct sockaddr*)&serv_addr,sizeof(serv_addr));//send filename

		if((str_len = recvfrom(sock,message,BUFSIZE,0,(struct sockaddr*)&serv_addr,&i)) != 0){
			//if received "received filename..."then file name sent successfully
			//if nothing is received send filname again
			printf("Sent Filename : %s\n",argv[3]);
			break;
		}
	}

	fcntl(sock,F_SETFL,flags & ~O_NONBLOCK); //change it back to blocking
	fp = fopen(argv[3],"rb");

	while(1){
		str_len = fread(message,1,BUFSIZE,fp);
		printf("Client : %d\n",str_len);
		if(str_len < BUFSIZE){
			sendto(sock,message,str_len,0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
			break;
		}
		sendto(sock,message,str_len,0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

	}
	printf("File all sent\n");
	fclose(fp);
	close(sock);
	return 0;
}
void error_handling(char *message)

{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);


}

