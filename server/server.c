#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>


//maximum data transfer
#define MESSAGE_LENTH 1024

//port number for connection
#define PORT 8888

//socket address
#define SA struct sockaddr

struct sockaddr_in serveraddress, client;

socklen_t length;

int socket_descriptor, connection, connection_status, bind_status;

char message[MESSAGE_LENTH];


int main(){
    //socket interface
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    //error check
    if(socket_descriptor == -1){
        printf("socket creation failed\n");
        _exit(1);
    }

    //initiliaze message array to 0's
    bzero(&serveraddress, sizeof(serveraddress));

    //server properties
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);

    //port
    serveraddress.sin_port = htons(PORT);

    //protocol family
    serveraddress.sin_family = AF_INET;


    //binding socket to the IP address
    bind_status = bind(socket_descriptor, (SA*)&serveraddress, sizeof(serveraddress));
    //error handling
    if(bind_status == -1){
        printf("Error, Binding socket failed\n");
        _exit(1);
    }

    //server listening for new connections
    connection_status = listen(socket_descriptor, 5);
    length = sizeof(client);

    if(connection_status == - 1){
        perror("Error, server is unable to listen for new request\n");
        _exit(1);
    }else{
        printf("Server is listening for new connections\n");
    }

    //client connection
    connection = accept(socket_descriptor, (SA*)&client, &length);
     if(connection == -1){
        perror("Error, server was unable to accept new client\n");
        _exit(1);
    }

    
   
    //connection loop
    while(1){

        bzero(message, MESSAGE_LENTH);

        //read data from client
        read(connection, message, sizeof(message));

        
        if(strncmp("end", message, 3) == 0){
            printf("Client exited.\n");
            printf("server exiting...\n");
            break;
        }

        printf("Data received from the client: %s\n", message);
        bzero(message, MESSAGE_LENTH);

        printf("Enter message to client\n");

        //scan message to client
        scanf("%[^\n]%*c", message);

        //write to client socket
        ssize_t bytes = write(connection, message, sizeof(message));

        if(bytes >= 0){
            printf("Data sent to client!\n");

        }



        
    }

    close(socket_descriptor);


    return 0;
}






