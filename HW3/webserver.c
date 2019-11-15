#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <string.h>
//WebServer
#define BUFFER 1024
void error_handling(char *message);

int main(int argc, char **argv){

	int serv_sock;
	int clnt_sock;
	FILE *fp;

	if(argc != 2){ 
		printf("Usage : %s <port_num>\n", argv[0]);
		exit(1);
	} 

	char message[BUFFER]={0};

	int str_len,i;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	serv_sock=socket(PF_INET,SOCK_STREAM, 0);
	char *method, *filename; 

	char *OK = "HTTP/1.1 200 OK\n\n";
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock,5)==-1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	while(1){

		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);

		if(clnt_sock==-1)
			error_handling("accept() error");

		read(clnt_sock,message,BUFFER);

		printf("%s\n",message);

		method= strtok(message," ");
		char *body;	
		if(strcmp(method,"POST")==0){
			for(i=0;i<14;i++){
				body = strtok(NULL,"\r\n");
		//		printf("%s\n",body);}
		
		}
			write(clnt_sock,OK,strlen(OK));
			write(clnt_sock,body,strlen(body));
		}
		if(strcmp(method,"GET")==0){
			filename = strtok(NULL, " ");
			printf("Ready to send %s to client...\n",filename);
			if(strcmp(filename,"/")==0){
				fp= fopen("/home/s21400547/Network/HW3/index.html","rb");}
			else{
			filename = strtok(filename,"/");
			fp = fopen(filename,"rb");
			}
			
			


			if(fp){
				write(clnt_sock,OK,strlen(OK));
				str_len = fread(message,sizeof(char),BUFFER,fp);
				write(clnt_sock,message,str_len);
				printf("%s",message);
			}
			else{
				if(strcmp(filename,"favicon.ico")==0)
					printf("Ignore favicon request!!!\n");
				else{
					write(clnt_sock,"HTTP/1.1 404 Not Found\n",23);

				}
			}

		}			

		printf("Response sent!\n");		

		close(clnt_sock);
	}
	return 0;
}

void error_handling(char *message)

{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);


}



