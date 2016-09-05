//
//  ClientSocket.cpp
//  Client_C_Socket
//
//  Created by Jake Sanders on 9/5/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "ClientSocket.hpp"

void ClientSocket::setSocket(std::string hostName, int portNum) {
    int connectionSocket; //This is a "file descriptor", which stores values from both the socket system call and the accept system call
    int portno; //The port nubmer where connections are accepted
    struct sockaddr_in serv_addr; //This is an object of the struct "sockaddr_in", which contains internet address. See the server side code for more details
    int n; //n stores the return value from the calls to read() and write() by holding the number of characters either read or written
    
    char buffer[256]; //This program will read characters from the connection into this buffer
    
    /* struct hostent
     
     struct hostent {
         char *h_name;           //Official name of the host
         char **h_aliases;       //A zero terminated array of alternate names for the host
         int h_addrtype;         //The type of address being returned; currently always AF_INET
         int h_length;           //The length, in bytes, of the address
         char **h_addr_list;     //A pointer to a list of network addresses for the named host. Host addresses are returned in network byte order.
     
     #define h_addr h addr_list[0]
     };
     
     */
    
    struct hostent *server; //A hostent object representing the server side.
    
    this->portNumber = portNum;
    
    /* socket()
     The socket() function returns a new socket, with three parameters.
     
     The first argument is address of the domain of the socket.
     The two possible address domains are the unix, for commen file system sockets, and the internet, for anywhere on the internet.
     AF_UNIX is generally used for the former, and AF_INET generally for the latter.
     
     The second argument is the type of the socket.
     The two possible types are a stream socket where characters are read in a continuous stream, and a diagram socket, which reads in chunks.
     SOCK_STREAM is generally used for the former, and SOCK_DGRAM for the latter.
     
     The third argument is the protocol. It should always be 0 except in unusual circumstances, and then allows the operating system to chose TCP or UDP, based on the socket type. TCP is chosen for stream sockets, and UDP for diagram sockets
     
     The function returns an integer than can be used like a reference to the socket. Failure results in returning -1.
     */
    connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    //Checks for errors initializing socket
    if (socket < 0)
        throw std::runtime_error("ERROR opening socket");
    
    /* gethostbyname()
     The gethostbyname() function returns a hostnet struct with the given name that provides info about that host, with one argument.
     
     The argument is the name of the host to locate.
     
     The property of the hostnet, *h_addr, is the IP address of the host
     
     If a host with that name couldn't be found, NULL is returned rather than the pointer.
     */
    server = gethostbyname(argv[1]);
    
    //Check for errors finding host
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    
    /* bzero()
     The bzero() function sets all values in a buffer to zero, with two parameters.
     
     The first argument is a pointer to the buffer.
     
     The second argument is the size of the buffer.
     
     This line essentially "initializes" serv_addr with zeros.
     */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    //The first property of the struct sockaddr_in should always be set to the symbolic constant AF_INET
    serv_addr.sin_family = AF_INET;
    
    /* bcopy()
     The bcopy() function copies a specified number of chars from one array to another, with three argument.
     
     The first argument is the first character array, to copy from.
     
     The second argument is the second character array, to copy to.
     
     The third argument is the number of characters to copy.
     
     This line copies the server's IP adress to the correct property in the address.
     */
    bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    
    //The second property contains the port numver, but it has to be converted to a network byte order
    serv_addr.sin_port = htons(portno);
    
    /* connect()
     The connect() function is called by the client to establish a connection with the server, with three arguments.
     
     The first argument is the small integer reference for the socket.
     
     The second argument is the address of the host to connect to, including the port number, all in the form of a sockaddr struct.
     
     The third argument is the size of the address.
     
     The function returns 0 if successful and -1 if it fails.
     */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    
    //Prompts the user to enter the message to be sent to the server
    printf("Please enter the message:\n");
    
    //Initialize the buffer to store the message to send
    bzero(buffer, 256);
    
    //Get a message from the console
    fgets(buffer, 255, stdin);
    
    /* write()
     The write() function will write a message to the client socket, with three arguments.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the message.
     
     The third argument is the length of the message.
     */
    n = write(sockfd, buffer, strlen(buffer));
    
    //Check for errors writing the message
    if (n < 0)
        error("ERROR writing to socket");
    
    //Reset the buffer to store the incoming message
    bzero(buffer, 256);
    
    /* read()
     The read() function will read in info from the client socket, with three arguments. It will block until the client writes and there is something to read in.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the buffer to store the message.
     
     The third argument is the maximum number of characters to to be read into the buffer.
     */
    n = read(sockfd, buffer, 255);
    
    //Check for errors reading in the message
    if (n < 0)
        error("ERROR reading from socket");
    
    //Print the received message
    printf("%s\n", buffer);
    
    //Properly terminate the socket
    close(sockfd);
    
    return 0;
}

void ServerSocket::send(std::string message) {
    char buffer[message.length()]; //This program will read characters from the connection into this buffer
    
    //Initialize the buffer where received info is stored
    bzero(buffer,256);
    
    long messageSize; //Stores the return value from the calls to read() and write() by holding the number of characters either read or written
    
    /* write()
     The write() function will write a message to the client socket, with three arguments.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the message.
     
     The third argument is the length of the message.
     */
    messageSize = write(this->clientSocket, message.c_str(), message.length());
    
    //Check for errors writing the message
    if (messageSize < 0)
        throw std::runtime_error("ERROR writing to socket");
}

std::string ServerSocket::receive() {
    char buffer[1024]; //This program will read characters from the connection into this buffer
    
    //Initialize the buffer where received info is stored
    bzero(buffer,1024);
    
    long messageSize; //Stores the return value from the calls to read() and write() by holding the number of characters either read or written
    
    /* read()
     The read() function will read in info from the client socket, with three arguments. It will block until the client writes and there is something to read in.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the buffer to store the message.
     
     The third argument is the maximum number of characters to to be read into the buffer.
     */
    messageSize = read(this->clientSocket, buffer, 1023);
    
    //Checks for errors reading from the socket
    if (messageSize < 0)
        throw std::runtime_error("ERROR reading from socket");
    
    //Return the received message to the console
    return std::string(buffer);
}

ServerSocket::~ServerSocket() {
    //Properly terminate the sockets
    close(this->clientSocket);
    close(this->hostSocket);
}