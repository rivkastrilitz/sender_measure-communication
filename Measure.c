
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <netinet/tcp.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/time.h>
#define PORT 8080 

void get_file(int new_socket, int name){
    struct stat st;
    int n;
    long get_mb=0;
    char buffer_1[20500];
    

    while(1){
        n = recv(new_socket,buffer_1,20500,0);
        if(n <= 0){
            break;
            return;
        }
        get_mb = n + get_mb;
        //set the buffer to 0
        bzero(buffer_1,20500);
    }
  printf("number of byts is: %ld\n",get_mb); 
    return;
}


int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from measure"; 
    double reno_time = 0, cubic_time = 0,time_keeper = 0 ;
    char buf[256];
    socklen_t len;
    long seconds, microseconds;
    struct timeval begin, end;
   

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("[-] socket failed"); 
        exit(EXIT_FAILURE); 
    } 
     printf("[+] socket created successfully.\n");  
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("[-] bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if(listen(server_fd, 10) == 0){
		printf("Listening....\n");
	}else{
		perror("[-] Error in listening");
    exit(1);
	}

    for (int i=0 ; i<5 ; i++)
    {
  
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 


    len = sizeof(buf); 
    if (getsockopt(server_fd, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
    perror("getsockopt");
    return -1;
    } 
    printf("[+] Current cc is: %s\n", buf); 

    // get the time that toke to get the file
    gettimeofday(&begin,0);
    get_file(new_socket,i);
    gettimeofday(&end,0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    time_keeper += (double)(seconds + microseconds*1e-6);
    
    }

    cubic_time = (time_keeper/5);

    time_keeper=0;

    strcpy(buf, "reno"); 
    len = strlen(buf);
     if (setsockopt(server_fd, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
     perror("setsockopt"); 
     exit(EXIT_FAILURE);
     }

    len = sizeof(buf); 
    if (getsockopt(server_fd, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
    perror("getsockopt");
    return -1;
    } 

    printf("\n[+] cc has been change to: %s\n", buf); 


    for (int i=0 ; i<5; i++)
    {

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
   

    len = sizeof(buf); 
    if (getsockopt(server_fd, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
    perror("getsockopt");
    return -1;
    } 
    printf("[+] Current cc is: %s\n", buf); 

     gettimeofday(&begin,0);
    get_file(new_socket,i);
    gettimeofday(&end,0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    time_keeper += (double)(seconds + microseconds*1e-6);
   
    }
   

    reno_time = (time_keeper/5);
    printf("\n[+] averge reno time: %lf \n[+] averge cubic time:%lf \n",reno_time,cubic_time);
    close(server_fd);
    printf("\n socket has been closed\n");
    return 0; 
    
}