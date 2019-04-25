#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERV_PORT	39407
#define BUF_SIZE	65000

using namespace std;

int main(int argc , char *argv[])
{
	// parameters
	int REPEAT_N = atoi(argv[1]);
	struct timespec tn, tn2;
	int N = 0;

	// socket setup
	//char inputBuffer[256] = {};
	//char message[] = {"Hi,this is server.\n"};
	char* recv_buf;
	recv_buf = (char*) malloc(BUF_SIZE*sizeof(char));
	int sockfd = 0,forClientSockfd = 0;
	sockfd = socket(AF_INET , SOCK_STREAM , 0);

	if (sockfd == -1){
		printf("Fail to create a socket.");
	}

	// socket connection
	struct sockaddr_in serverInfo,clientInfo;
	int addrlen = sizeof(clientInfo);
	bzero(&serverInfo,sizeof(serverInfo));

	serverInfo.sin_family = PF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(SERV_PORT);
	bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
	listen(sockfd,5);

	forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, (socklen_t *) &addrlen);
	clock_gettime(CLOCK_REALTIME, &tn);
	while(1){
		recv(forClientSockfd, recv_buf, sizeof(recv_buf), 0);
		//send(forClientSockfd,message,sizeof(message),0);
		++N;
		if (N % REPEAT_N == 0)
		{
			clock_gettime(CLOCK_REALTIME, &tn2);
			double diff = tn2.tv_sec * 1000000000 + tn2.tv_nsec - (tn.tv_sec * 1000000000 + tn.tv_nsec);
			double throughput = REPEAT_N * BUF_SIZE * sizeof(char) / diff * 8 / 1.073741824;
			cerr << throughput << endl;
			tn = tn2;
		}
	}
	return 0;
}
