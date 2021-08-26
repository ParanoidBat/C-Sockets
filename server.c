#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main (int argc, char const *argv[]){
  int opt = 1;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  int new_socket, value_read;
  char buffer[1024] = {0};
  char *message = "I am server man";

  /*
    AF_INET = IPv4
    SOCK_STREAM = TCP

  */
  int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_descriptor < 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }


  /*
    set options at socket level.
    make socket reusable
  */
  if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0){
    perror("socket options");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // accept request from any address
  address.sin_port = htons(PORT); // htons() converts short int from host byte order to network byte order

  // bind a name to the socket and attach to port
  if(bind(socket_descriptor, (struct sockaddr *)&address, sizeof(address) ) < 0){
    perror("binding error");
    exit(EXIT_FAILURE);
  }

  // ready to listen at specified socket, connection request queue is set to 3
  if(listen(socket_descriptor, 3) < 0){
    perror("listening error");
    exit(EXIT_FAILURE);
  }


  // accept new connection on socket. create a new socket with same properties as provided socket and return it to caller
  if ((new_socket = accept(socket_descriptor, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
    perror("acceptance error");
    exit(EXIT_FAILURE);
  }


value_read = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );

    send(new_socket , message , strlen(message) , 0 );
    printf("Message sent from server\n");

  return 0;
}