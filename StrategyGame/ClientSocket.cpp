//
//  ClientSocket.cpp
//  Client_C_Socket
//
//  Created by Jake Sanders on 9/5/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "ClientSocket.hpp"

ClientSocket::ClientSocket() {}

void ClientSocket::setSocket(std::string hostName, int portNum) {
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
    this->connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    //Checks for errors initializing socket
    if (socket < 0)
        throw std::runtime_error("ERROR opening socket");
    
    /* gethostbyname()
     The gethostbyname() function returns a hostnet struct with the given name that provides info about that host, with one argument.
     
     The argument is the name of the host to locate.
     
     The property of the hostnet, *h_addr, is the IP address of the host
     
     If a host with that name couldn't be found, NULL is returned rather than the pointer.
     */
    
    this->server = gethostbyname(hostName.c_str());
    
    //Check for errors finding host
    if (this->server == NULL) {
        std::runtime_error("ERROR, no such host");
    }
    
    /* bzero()
     The bzero() function sets all values in a buffer to zero, with two parameters.
     
     The first argument is a pointer to the buffer.
     
     The second argument is the size of the buffer.
     
     This line essentially "initializes" serv_addr with zeros.
     */
    bzero((char *) &this->serverAddress, sizeof(this->serverAddress));
    
    //The first property of the struct sockaddr_in should always be set to the symbolic constant AF_INET
    this->serverAddress.sin_family = AF_INET;
    
    /* bcopy()
     The bcopy() function copies a specified number of chars from one array to another, with three argument.
     
     The first argument is the first character array, to copy from.
     
     The second argument is the second character array, to copy to.
     
     The third argument is the number of characters to copy.
     
     This line copies the server's IP adress to the correct property in the address.
     */
    bcopy((char *)this->server->h_addr, (char *) &this->serverAddress.sin_addr.s_addr, this->server->h_length);
    
    //The second property contains the port numver, but it has to be converted to a network byte order
    this->serverAddress.sin_port = htons(this->portNumber);
    
    /* connect()
     The connect() function is called by the client to establish a connection with the server, with three arguments.
     
     The first argument is the small integer reference for the socket.
     
     The second argument is the address of the host to connect to, including the port number, all in the form of a sockaddr struct.
     
     The third argument is the size of the address.
     
     The function returns 0 if successful and -1 if it fails.
     */
    if (connect(this->connectionSocket, (struct sockaddr *) &this->serverAddress, sizeof(this->serverAddress)) < 0)
        throw std::runtime_error("ERROR connecting");
    
    this->setUp = true;
}

void ClientSocket::send(std::string message) {
    if (!this->setUp)
        throw std::logic_error("Socket not set");
    
    char buffer[message.length() + 1];
    
    //Initialize the buffer to store the message to send
    bzero(buffer, message.length() + 1);
    
    for (int stringIndex = 0; stringIndex < message.length(); stringIndex++) {
        buffer[stringIndex] = message[stringIndex];
    }
    
    //Add a terminating character
    buffer[message.length()] = -1;
    
    long messageSize; //Stores the return value from the calls to read() and write() by holding the number of characters either read or written
    
    /* write()
     The write() function will write a message to the client socket, with three arguments.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the message.
     
     The third argument is the length of the message.
     */
    messageSize = write(this->connectionSocket, buffer, strlen(buffer));
    
    //Check for errors writing the message
    if (messageSize < 0)
        throw std::runtime_error("ERROR writing to socket");
    
#ifdef SOCKET_CONSOLE_OUTPUT
    std::cout << "Client sent:" << std::endl << "\t";
    
    for (int a = 0; a < MAXIMUM_SOCKET_MESSAGE_SIZE; a++) {
        if ((int)buffer[a] == -1) {
            std::cout << (int)buffer[a];
            break;
        }
        
        std::cout << (int)buffer[a] << " ";
    }
    
    std::cout << std::endl << "\t\"" << buffer << "\"" << std::endl;
#endif

}

std::string ClientSocket::receive() {
    if (!this->setUp)
        throw std::logic_error("Socket not set");
    
    char buffer[MAXIMUM_SOCKET_MESSAGE_SIZE];
    
    //Initialize the buffer to store the message to send
    bzero(buffer, MAXIMUM_SOCKET_MESSAGE_SIZE);
    
    long messageSize; //Stores the return value from the calls to read() and write() by holding the number of characters either read or written
    
    /* read()
     The read() function will read in info from the client socket, with three arguments. It will block until the client writes and there is something to read in.
     
     The first argument is the reference for the client's socket.
     
     The second argument is the buffer to store the message.
     
     The third argument is the maximum number of characters to to be read into the buffer.
     */
    messageSize = read(this->connectionSocket, buffer, MAXIMUM_SOCKET_MESSAGE_SIZE - 1);
    
    //Check for errors reading in the message
    if (messageSize < 0)
        throw std::runtime_error("ERROR reading from socket");
    
#ifdef SOCKET_CONSOLE_OUTPUT
    std::cout << "Client received:" << std::endl << "\t";
    
    for (int a = 0; a < MAXIMUM_SOCKET_MESSAGE_SIZE; a++) {
        if ((int)buffer[a] == -1) {
            std::cout << (int)buffer[a];
            break;
        }
        
        std::cout << (int)buffer[a] << " ";
    }
    
    std::cout << std::endl << "\t\"" << buffer << "\"" << std::endl;
#endif
    
    std::string message;
    
    for (int iterator = 0; iterator < MAXIMUM_SOCKET_MESSAGE_SIZE; iterator++) {
        if (buffer[iterator] == -1)
            break;
        else
            message.push_back(buffer[iterator]);
    }
    
    //Return the received message
    return message;
}

ClientSocket::~ClientSocket() {
    if (this->setUp)
        //Properly terminate the sockets
        close(this->connectionSocket);
}
