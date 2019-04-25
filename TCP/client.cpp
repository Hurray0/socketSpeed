#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
#include <time.h>

#define SERV_PORT	39407
#define BUF_SIZE	1000000

using namespace std;

int main(int argc , char *argv[])
{
    // parameters
    struct timespec tn, tn2;
    int REPEAT_N = atoi(argv[2]);
    int REPEAT_M = atoi(argv[3]);
    float SLEEP = atof(argv[4]);

    // socket setup
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    // socket connection
    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    info.sin_addr.s_addr = inet_addr(argv[1]); // server ip
    info.sin_port = htons(SERV_PORT); // server port


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
        printf("Connection error");
    }

    // Send a message to server
    //char send_buf[] = {"Hi there"};
    //char receiveMessage[100] = {};
    char *send_buf;
    send_buf = (char *)malloc(BUF_SIZE*sizeof(char));
    cout << sizeof(char) * BUF_SIZE << endl;
    for (int j = 0; j < REPEAT_M; j++) {
	    clock_gettime(CLOCK_REALTIME, &tn);
	    for (int i = 0; i < REPEAT_N; i++) {
		    send(sockfd, send_buf, BUF_SIZE*sizeof(char),0);
	    }

	    clock_gettime(CLOCK_REALTIME, &tn2);
	    double start_time = tn.tv_sec * 1000000000 + tn.tv_nsec;
	    double end_time = tn2.tv_sec * 1000000000 + tn2.tv_nsec;
	    double diff = end_time - start_time;
	    double throughput = REPEAT_N*BUF_SIZE*sizeof(char)/diff*8/1.024/1.024/1.024;
	    //printf("time = %lfns, buf = %dbits, throughput = %lfGbps\n", diff, sizeof(send_buf), throughput);
	    cout << "time = " << diff << "ns, buf = " << REPEAT_N*BUF_SIZE*sizeof(char) << "bits, throughput = " << throughput << "Gbps" << endl;
    }
    //recv(sockfd,receiveMessage,sizeof(receiveMessage),0);

    printf("close Socket\n");
    close(sockfd);
    return 0;
}
