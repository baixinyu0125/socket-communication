#include "net_exp.h"

#define MAXPENDING 5


void quit_tranmission(int sig){
	printf("re a quit signal = %d\n",sig);
	exit(EXIT_SUCCESS);
}


void DieWithError(char *errorMessage){

	perror(errorMessage);

	exit(1);

}



void Print(char *errorMessage){

	perror(errorMessage);

}



int main(int argc, char **argv)

{

    struct sockaddr_in serverAddr, clientAddr;

    int serverSock, clientSock;

    int recvMsgSize;

    unsigned int clientLen;

    char recv_buf[TCP_BUF_LENGTH]; 

    int pkt_len = 0;

    int on = 1;

    if ((serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)

        DieWithError("socket() failed.");

    memset(&serverAddr, 0, sizeof(serverAddr));    

    serverAddr.sin_family = AF_INET;

    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    serverAddr.sin_port = htons(TCP_SERVER_PORT);
    if(setsockopt(serverSock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
	 DieWithError("socket() failed.");
	

    if (bind(serverSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)

        DieWithError("bind() failed.");

    if (listen(serverSock, MAXPENDING) < 0)

        DieWithError("listen() failed");


        clientLen = sizeof(clientAddr);  //队列长度


        if ((clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &clientLen)) < 0)

            DieWithError("accept() failed");


	pid_t pid;
	pid = fork();
	if(pid == -1)
	    DieWithError("fork() failed");

	if(pid == 0){
	signal(SIGUSR1,quit_tranmission);
	char sendbuf[256] = {0};
	while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL){
	  write(clientSock,sendbuf,strlen(sendbuf));
	  bzero(sendbuf,strlen(sendbuf));
		}
	exit(1);
	}
	else{
	while(1){
	bzero(recv_buf,strlen(recv_buf));
	 recvMsgSize = recv(clientSock, recv_buf, TCP_BUF_LENGTH, 0);
	 if (recvMsgSize < 0)

         	DieWithError("recv() failed.");
	else if(recvMsgSize == 0){
		printf("close!");
		break;
	}
	fputs(recv_buf,stdout);
		}
	kill(pid,SIGUSR1);
	}



    close(clientSock);
    close(serverSock);

}    




