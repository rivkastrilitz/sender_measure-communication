
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/stat.h>
#include <netinet/tcp.h> 
#define PORT 8080 


void send_file(FILE *fp,int sock){

    char buffer_1[20500] = {0}; 

    //fgets-read a line and store in buffer
    while( fgets(buffer_1,20500,fp) != NULL){
        if(send(sock,buffer_1,sizeof(buffer_1),0) == -1){
            perror("error in reading data"); 
             exit(1); 
        }
       
        bzero(buffer_1,20500);
    }
}

   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    FILE *fp;
    char *file_name="1mb.txt";
    struct stat st;
    char buf[256];
    socklen_t len;
   
    
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\n[-]Invalid address/ Address not supported \n"); 
        return -1; 
    } 


  for(int i =0 ; i<10 ; i++)
  {      // create tcp socket
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n [-] Socket creation error \n"); 
        return -1; 
    } 
    printf("[+] socket created successfully.\n");
   
       fp =fopen(file_name,"r");
    if(fp==NULL){
        perror("[-] error in reading file "); 
        exit(1); 
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\n[-] connection Failed \n"); 
        return -1; 
    } 
    printf("[+] Connected to measure.\n");

    if(i>=5)
    {
   strcpy(buf, "reno"); 
    len = strlen(buf);
     if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
     perror("setsockopt"); 
     exit(EXIT_FAILURE);
     }
    }

    len = sizeof(buf); 
    if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
    perror("getsockopt");
    return -1;
    } 
    printf("[+] Current cc is: %s\n", buf); 


   send_file(fp,sock);

    printf("[+] send file finish\n");
    close(sock);
}
printf("[+] socket is closed\n");
    return 0; 
} 
