//
//  ClientSocket.hpp
//  Client_C_Socket
//
//  Created by Jake Sanders on 9/5/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef ClientSocket_hpp
#define ClientSocket_hpp

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
    void setSocket(std::string hostName, int portNum);
    
    void send(std::string message);
    
    std::string receive();
    
private:
    //Private properties
    
    int connectionSocket; //This is the "file descriptor", which stores values from both the socket system call and the accept system call
    int portNumber; //The port nubmer where connections are accepted
    struct sockaddr_in serverAddress; //This is an object of the struct "sockaddr_in", which contains internet address. See the server side code for more details
    
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
    
    struct hostent *server;
};

#endif /* ClientSocket_hpp */
