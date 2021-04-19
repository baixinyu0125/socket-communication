#include "net_exp.h"



void DieWithError(char *errorMessage){

	perror(errorMessage);

	exit(1);

}


void quit_tranmission(int sig){
	printf("re a quit signal = %d\n",sig);
	exit(EXIT_SUCCESS);
}


void Print(char *errorMessage){

	perror(errorMessage);

}



int main(int argc, char **argv)

{

	struct sockaddr_in serverAddr;

	int clientSock;

	char sendbuf[TCP_BUF_LENGTH] = "test";



	char recv_buf[TCP_BUF_LENGTH]; 
	

	int pkt_len = 0;

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(TCP_SERVER_PORT);

	serverAddr.sin_addr.s_addr =  inet_addr(TCP_SERVER_ADDRESS);


	if((clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)

		DieWithError("socket() failed");
	int conn;
	conn = connect(clientSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	if(conn < 0)

	{

		  DieWithError("connect() failed");

	}
	pid_t pid;
	pid = fork();
	if(pid == -1)
	    DieWithError("fork() failed");
	if(pid == 0){
	 while(1){
	 bzero(recv_buf,strlen(recv_buf));
	if((pkt_len = recv(clientSock, recv_buf, TCP_BUF_LENGTH, 0)) < 0)

		DieWithError("sender: recv() failed.");	



		else if(pkt_len == 0) 

		{
		printf("Close!");

		break;

		}
		fputs(recv_buf,stdout);
	}
	close(clientSock);
	kill(getppid(),SIGUSR1);
	exit(1);
	}
	else{
	signal(SIGUSR1,quit_tranmission);
	while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL){
	 if (send(clientSock, sendbuf, strlen(sendbuf), 0) < 0)

		DieWithError("send() error.");
	  bzero(sendbuf,strlen(sendbuf));
		}
	close(clientSock);
}


	return 0;

}
