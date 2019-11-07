#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <string.h>


#define BUFSIZE 30
	void error_handling(char *message);
	int main(int argc, char **argv){
	int sock;
	char message[BUFSIZE];
	int str_len, addr_size, i;
	char MSG1[] = "0123456789\n";
	char MSG2[] = "ABCDEFGHIJ\n";
	char MSG3[] = "KLMNOPQRST\n";
	struct sockaddr_in serv_addr;
	struct sockaddr_in from_addr;
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handling("UDP socket not made");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	sendto(sock, MSG1, strlen(MSG1), 0, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
	sendto(sock, MSG2, strlen(MSG2), 0, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
	sendto(sock, MSG3, strlen(MSG3), 0, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
	//connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	while(1)
		{
		fputs("input message (q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message,"q\n")){
			sendto(sock, message,BUFSIZE,0, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
			break;
		}
		sendto(sock, message, BUFSIZE, 0, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
		//write(sock, message, strlen(message));
		}

	close(sock);
	return 0;
}
void error_handling(char *message)

{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);


}
