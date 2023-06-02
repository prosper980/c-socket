#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


//maximum data transfer
#define MESSAGE_LENGHT 1024

//Port number
#define PORT 8888

//socket address
#define SA struct sockaddr

//global data
int socket_fd, connection;

struct sockaddr_in server_address, client;

char message[MESSAGE_LENGHT];


int main(){

    //socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd == -1){
        printf("Error, Unable to create socket\n");
        _exit(1);
    }


    //erasing the memory
    bzero(&server_address, sizeof(server_address));


    //server properties
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    //setting the port number
    server_address.sin_port = htons(PORT);

    //protocol family
    server_address.sin_family = AF_INET;

    //establishing connection with the server 
    connection = connect(socket_fd, (SA*)&server_address, sizeof(server_address));

    if(connection == -1){
        printf("Connecting to server failed with server failed\n");
        _exit(1);
    }

    while(1){
        bzero(message, sizeof(message));

        printf("Enter the message you want to send to the server\n");

        scanf("%[^\n]%*c", message);
        //message to server
        ssize_t bytes = write(socket_fd, message, sizeof(message));

        if(bytes <= 0 ){
            printf("Data sent to the server\n");
        }

        //erazing the memory
        bzero(message, sizeof(message));

        //reading response from server 
        read(socket_fd, message,sizeof(message));

        if(strncmp(message, "end", 3) == 0 ){
            write(socket_fd, message, sizeof(message));
            printf("Client exiting...\n");
             break;
        }

        printf("Data received from server:  %s\n", message);

    }

    //closing the connection on exit
    close(socket_fd);

    return 0;
}