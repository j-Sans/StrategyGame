//
//  ServerSocket.cpp
//  Server_C_Socket
//
//  Created by Jake Sanders on 9/5/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "ServerSocket.hpp"

ServerSocket::ServerSocket() {}

void ServerSocket::setSocket(int portNum) {
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
    this->hostSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    //If socket() returns an error, -1.
    if (this->hostSocket < 0)
        throw std::runtime_error("Error opening socket");
    
    /* bzero()
     The bzero() function sets all values in a buffer to zero, with two parameters.
     
     The first argument is a pointer to the buffer.
     
     The second argument is the size of the buffer.
     
     This line essentially "initializes" serverAddress with zeros.
     */
    bzero((char *) &this->serverAddress, sizeof(this->serverAddress));
    
    this->portNumber = portNum;
    
    //The first property of the struct sockaddr_in should always be set to the symbolic constant AF_INET
    this->serverAddress.sin_family = AF_INET;
    
    //The second property contains the port numver, but it has to be converted to a network byte order
    this->serverAddress.sin_port = htons(this->portNumber);
    
    //The third property is a struct with one property, an unsigned long, and it contains the host IP address. Server side, this will always be the current IP address of the machine. The symbolic constant INADDR_ANY gets this address.
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    /* bind()
     The bind() function connects a socket to an address, with three parameters.
     Here it will connect the socket to the host at the proper port number.
     
     The first argument is the socket, by its simple integer reference.
     
     The second argument is a pointer to the address, contained as a sockaddr_in struct. It must be cast properly.
     
     The third argument is the size of the sockaddr_in struct that is passed in by reference.
     
     This function can fail for multiple reasons. The most likely one is if the port is already in use.
     */
    if (bind(this->hostSocket, (struct sockaddr *) &this->serverAddress, sizeof(this->serverAddress)) < 0)
        throw std::runtime_error("ERROR binding socket to host");
    
    /* listen()
     The listen() function listens for connections on the socket, with two arguments.
     
     The first argument is the socket, by its simple integer reference.
     
     The second argument is the size of the "backlog queue", or the number of connections that can be waiting as another connection is handled. 5 is the max for most systems, so 5 should be set.
     
     This function cannot fail, as long as the socket is valid, so there is no error code.
     */
    listen(this->hostSocket, 5);
    
    this->setUp = true;
}

void ServerSocket::addClient() {
    if (this->connectedClients >= 5)
        throw std::range_error("Cannot connect more than five socekts");
    
    //this->connectedClients represents the number of connected clients, and also the next open index of sockets
    
    this->clientAddressSizes[this->connectedClients] = sizeof(this->clientAddresses[this->connectedClients]);
    
    /* accept()
     The accept() function makes the process block until a connection is formed between the client and the server, with three arguments. It then wakes when the connection is successfully established.
     
     The first argument is the host side socket, passed by its reference.
     
     The second argument is a reference to the address of the client, in the form of a sockaddr struct pointer.
     
     The third argument is the size of the struct, passed by value.
     
     The return value is a socket, passed by a small integer reference.
     */
    this->clientSockets[this->connectedClients] = accept(this->hostSocket, (struct sockaddr *) &this->clientAddresses[this->connectedClients], &this->clientAddressSizes[this->connectedClients]);
    
    //Checks for error with accepting
    if (this->clientSockets[this->connectedClients] < 0)
        throw std::runtime_error("ERROR accepting client");
    
    this->connectedClients++;
}

void ServerSocket::send(std::string message, unsigned int clientIndex) {
    if (!this->setUp)
        throw std::logic_error("Socket not set");
    
    if (clientIndex >= connectedClients)
        throw std::range_error("Socket index uninitialized");
    
    char buffer[message.length() + 1]; //This program will read characters from the connection into this buffer
    
    //Initialize the buffer where received info is stored
    bzero(buffer, message.length() + 1);
    
    for (int stringIndex = 0; stringIndex < message.length(); stringIndex++) {
        buffer[stringIndex] = message[stringIndex];
    }
    
    //Add a terminating character
    buffer[message.length()] = (char)4;
    
    long messageSize; //Stores the return value from the calls to read() and write() by holding the number of characters either read or written
    
    /* write()
     The write() function will write a message to the client socket, with three arguments.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the message.
     
     The third argument is the length of the message.
     */
    messageSize = write(this->clientSockets[index], buffer, strlen(buffer));
    
    //Check for errors writing the message
    if (messageSize < 0)
        throw std::runtime_error("ERROR writing to socket");
    
#ifdef SOCKET_CONSOLE_OUTPUT
    std::cout << "Host sent: \"" << message << "\"" << std::endl << std::endl;
#endif
}

void ServerSocket::broadcast(std::string message) {
    if (!this->setUp)
        throw std::logic_error("Socket not set");
    
    if (clientIndex >= connectedClients)
        throw std::range_error("Socket index uninitialized");
    
    char buffer[message.length() + 1]; //This program will read characters from the connection into this buffer
    
    //Initialize the buffer where received info is stored
    bzero(buffer, message.length() + 1);
    
    for (int stringIndex = 0; stringIndex < message.length(); stringIndex++) {
        buffer[stringIndex] = message[stringIndex];
    }
    
    //Add a terminating character
    buffer[message.length()] = (char)4;
    
    long messageSize; //Stores the return value from the calls to read() and write() by holding the number of characters either read or written
    
    /* write()
     The write() function will write a message to the client socket, with three arguments.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the message.
     
     The third argument is the length of the message.
     */
    for (int socket = 0; socket < this->connectedClients; socket++)
        messageSize = write(this->clientSockets[socket], buffer, strlen(buffer));
    
    //Check for errors writing the message
    if (messageSize < 0)
        throw std::runtime_error("ERROR writing to socket");
    
#ifdef SOCKET_CONSOLE_OUTPUT
    std::cout << "Host sent: \"" << message << "\"" << std::endl << std::endl;
#endif
}

std::string ServerSocket::receive(unsigned int clientIndex) {
    if (!this->setUp)
        throw std::logic_error("Socket not set");
        
    char buffer[MAXIMUM_SOCKET_MESSAGE_SIZE]; //This program will read characters from the connection into this buffer
    
    //Initialize the buffer where received info is stored
    bzero(buffer, MAXIMUM_SOCKET_MESSAGE_SIZE);
    
    long messageSize; //Stores the return value from the calls to read() and write() by holding the number of characters either read or written
    
    /* read()
     The read() function will read in info from the client socket, with three arguments. It will block until the client writes and there is something to read in.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the buffer to store the message.
     
     The third argument is the maximum number of characters to to be read into the buffer.
     */
    messageSize = read(this->clientSockets[clientIndex], buffer, MAXIMUM_SOCKET_MESSAGE_SIZE - 1);
    
    //Checks for errors reading from the socket
    if (messageSize < 0)
        throw std::runtime_error("ERROR reading from socket");
    
    std::string message;
    
    for (int iterator = 0; iterator < MAXIMUM_SOCKET_MESSAGE_SIZE; iterator++) {
        if (buffer[iterator] == (char)4)
            break;
        else
            message.push_back(buffer[iterator]);
    }
    
#ifdef SOCKET_CONSOLE_OUTPUT
    std::cout << "Host received: \"" << message << "\"" << std::endl << std::endl;
#endif
    
    //Return the received message to the console
    return message;
}

ServerSocket::~ServerSocket() {
    if (this->setUp) {
        //Properly terminate the sockets
        close(this->clientSocket);
        close(this->hostSocket);
    }
}
