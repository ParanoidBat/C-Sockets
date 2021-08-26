#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 80

int main(){
  struct sockaddr_in server_address;
  char message[100];
  char buffer[1024] = {0};
  int value_read;

  int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_descriptor < 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);

  // convert internet address from text to binary
  if(inet_pton(AF_INET, "192.168.4.1", &server_address.sin_addr) <= 0){
    perror("conversion failed");
    exit(EXIT_FAILURE);
  }

  // connect call automatically binds the socket if not bound previously.
  if(connect(socket_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
    perror("connection failed");
    exit(EXIT_FAILURE);
  }
  
  while(1){
    fgets(message, 100, stdin);
    send(socket_descriptor , message , strlen(message) , 0 );
    // printf("Message sent from client\n");
    value_read = read( socket_descriptor , buffer, 1024);
    if(buffer == "bye") break;
    printf("%s\n",buffer );
  }
  close(socket_descriptor);

  return 0;
}