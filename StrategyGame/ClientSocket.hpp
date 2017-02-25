//
//  ClientSocket.hpp
//  Client_C_Socket
//
//  Created by Jake Sanders on 9/5/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef ClientSocket_hpp
#define ClientSocket_hpp

#include "Macros.h"

#include <iostream>
#include <string>
#include <exception>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class ClientSocket {
public:
    //Constructor
    ClientSocket();
    
    //Destructor
    ~ClientSocket();
    
    //Public properties
    
    //Public member functions
    
    /*!
     * A function to initialize the socket. This must be done before the socket can be used. Will throw an error if the socket cannot be opened or if the port is occupied.
     *
     * @param hostName A const char* indicating the name of the host to whom to connect. "localhost" specifies that the host is on the same machine. Otherwise, use the name of the client.
     * @param portNum The number of the port on the host at which clients should connect.
     */
    void setSocket(const char* hostName, int portNum);
    
    /*!
     * A function that sends a message to the host. An error will be thrown if the socket is not set or if an error occurs in sending the message.
     *
     * @param message A std::string of the message to be sent.
     * @param throwErrorIfNotFullySent An optional boolean indicating if an error should be thrown if only part of the message was sent. Automatically set to false.
     */
    void send(const char* message, bool throwErrorIfNotFullySent = false);
    
    /*!
     * A function that receives a message from the host. The function will wait for a short period for the client to send the message, and if the message is not received it will throw an error. An error is also thrown if the socket is not set.
     *
     * @param socketClosed An optional pointer to a bool that would be set to true if the client disconnected. Automatically set to a null pointer otherwise.
     *
     * @return The received message from the host.
     */
    std::string receive(bool* socketClosed = nullptr);
    
    /*!
     * @return If this object is set.
     */
    bool getSet();
    
private:
    //Private properties
    
    /* struct addrinfo {
     int ai_flags; //AI_PASSIVE, AI_CANONNAME, etc...
     int ai_family; //AF_INET, AF_INET6, AF_UNSPEC. This is the type of IP address (IPv4, IPv6, or any, respectively)
     int ai_socktype; //SOCK_STREAM, SOCK_DGRAM (TCP and UDP, respectively)
     int ai_protocol; //0 represents any
     size_t ai_addrlen; //Represents the size of the following property ai_addr, in bytes
     struct sockaddr* ai_addr; //A struct containing the address info. Can be a sockaddr_in or sockaddr_in6
     char* hostname; //Full hostname
     
     struct addrinfo* ai_next; //The next addrinfo node. Used when addrinfo structs are returned as a linked list
     }
     */
    
    
    /* struct sockaddr {
     unsigned short sa_family; //Address family, meaning AF_INET or AF_INET6
     char sa_data[14]; //A destination address and a port number for the socket, in 14 bytes.
     */
    
    int connectionSocket; //This is the "file descriptor", which stores values from both the socket system call and the accept system call
    int portNumber; //The port nubmer where connections are accepted
    
    bool setUp = false; //Represents if the socket has already been set. If not, reading and writing will cause errors
};

#endif /* ClientSocket_hpp */
