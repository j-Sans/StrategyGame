//
//  ServerSocket.hpp
//  Server_C_Socket
//
//  Created by Jake Sanders on 9/5/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef ServerSocket_hpp
#define ServerSocket_hpp

#include "Macros.h"

#include <iostream>
#include <string>
#include <exception>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
 Make it so it broadcasts not only to itself
 Multiple people can run from same port
 
 Set up queue (FIFO)
 */

class ServerSocket {
public:
    //Constructor
    ServerSocket();
    
    //Destructor
    ~ServerSocket();
    
    //Public properties
    
    //Public member functions
    void setSocket(int portNum);
    
    void send(std::string message);
    
    std::string receive();
    
private:
    //Private properties
    unsigned int connectedClients = 0;
    
    //These are "file descriptors", which store values from both the socket system call and the accept system call
    int hostSocket;
    int clientSockets[5];
    int portNumber; //The port nubmer where connections are accepted
    socklen_t clientAddressSizes[5]; //The size of the address of the client, for the accept system call
    
    /* struct sockaddr_in
     
     struct sockaddr_in {
         short sin_family; //Should be AF_INET, see below
         u_short sin_port;
         struct in_addr sin_addr;
         char sin_zero[8]; //Not used, should be zero, see below
     };
     
     Note: the struct in_addr contains only a single property, an unsigned long named s_addr.
     */
    
    //These are objects of the struct "sockaddr_in", which contain internet addresses
    sockaddr_in serverAddress;
    sockaddr_in clientAddresses[5];
    
    bool setUp = false; //Represents if the socket has already been set. If not, reading and writing will cause errors
    
};

#endif /* ServerSocket_hpp */
