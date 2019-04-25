#include <stdio.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <unistd.h>   
#include <errno.h>   
#include <string.h>   
#include <stdlib.h>   
#include <time.h>
#include <iostream>

#define SERV_PORT   8000   
#define BUF_SIZE 65000

using namespace std;

int main(int argc, char* argv[])  
{  
	int REPEAT_N = atoi(argv[1]);
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	int N = 0;
	if(sock_fd < 0)  
	{  
		perror("socket");  
		exit(1);  
	}  

	struct sockaddr_in addr_serv;  
	int len;  
	memset(&addr_serv, 0, sizeof(struct sockaddr_in));  
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(SERV_PORT);
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
	len = sizeof(addr_serv); 

	if(bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)  
	{  
		perror("bind error:");  
		exit(1);  
	}  


	int recv_num;  
	char * recv_buf;
	struct sockaddr_in addr_client;  

	recv_buf = (char *)malloc(BUF_SIZE*sizeof(char));

	struct timespec tn, tn2;
	clock_gettime(CLOCK_REALTIME, &tn);
	while(1)  
	{  
		recv_num = recvfrom(sock_fd, recv_buf, BUF_SIZE*sizeof(char), 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);  
		if(recv_num > 0)
		{
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

		//if(recv_num < 0)  
		//{  
		//  perror("recvfrom error:");  
		//  exit(1);  
		//}  

		//recv_buf[recv_num] = '\0';  
		//printf("server receive %d bytes: %s\n", recv_num, recv_buf);  
	}  

	close(sock_fd);  

	return 0;  
}
