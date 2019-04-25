#include <stdio.h>   
#include <iostream>
#include <string.h>   
#include <errno.h>   
#include <stdlib.h>   
#include <unistd.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <time.h>
   
using namespace std;
  
#define DEST_PORT 8000   
//#define DSET_IP_ADDRESS  "127.0.0.1"   
#define BUF_SIZE 65000
//#define REPEAT_N 1

   
  
int main(int argc, char* argv[])  
{
  int sock_fd;
  struct timespec tn, tn2;
  int REPEAT_N = atoi(argv[2]);
  int REPEAT_M = atoi(argv[3]);
  float SLEEP = atof(argv[4]);
  
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);  
  if(sock_fd < 0)  
  {  
    perror("socket");  
    exit(1);  
  }  
    
  struct sockaddr_in addr_serv;  
  int len;  
  memset(&addr_serv, 0, sizeof(addr_serv));  
  addr_serv.sin_family = AF_INET;  
  //addr_serv.sin_addr.s_addr = inet_addr(DSET_IP_ADDRESS);  
  addr_serv.sin_addr.s_addr = inet_addr(argv[1]);  
  addr_serv.sin_port = htons(DEST_PORT);  
  len = sizeof(addr_serv);  
  
    
  int send_num;  
  //char send_buf[BUF_SIZE] = "hey, who are you?";  
  char *send_buf;
  send_buf = (char *)malloc(BUF_SIZE*sizeof(char));
      
	int n = 0;
	for (int j = 0; j < REPEAT_M; j++) {
	  clock_gettime(CLOCK_REALTIME, &tn);
	  for (int i = 0; i < REPEAT_N; i++) {  
		  send_num = sendto(sock_fd, send_buf, BUF_SIZE*sizeof(char), 0, (struct sockaddr *)&addr_serv, len);  
		  //printf("client send %d bytes: %s\n", send_num, send_buf);  
	  }
	  clock_gettime(CLOCK_REALTIME, &tn2);
	  double start_time = tn.tv_sec * 1000000000 + tn.tv_nsec;
	  double end_time = tn2.tv_sec * 1000000000 + tn2.tv_nsec;
	  double diff = end_time - start_time;
	  double throughput = REPEAT_N*BUF_SIZE*sizeof(char)/diff*8/1.024/1.024/1.024;
	  //printf("time = %lfns, speed = %lfGbps\n", diff, REPEAT_N*BUF_SIZE*sizeof(char)/diff*8);
	  //printf("time = %lfns, speed = %lfGbps\n", diff, REPEAT_N*BUF_SIZE*sizeof(char)/diff*1000000000/1024/1024/1024*8);
	  //cerr << throughput << endl;
		n++;
		if (n % 100 == 0)
			cout << n << endl;
	  sleep(SLEEP);
}
    
  if(send_num < 0)  
  {  
    perror("sendto error:");  
    exit(1);  
  }  
    
  close(sock_fd);  
    
  return 0;  
}
