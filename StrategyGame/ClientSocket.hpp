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
     * @param hostName A std::string indicating the name of the host to whom to connect. "localhost" specifies that the host is on the same machine. Otherwise, use the name of the client.
     * @param portNum The number of the port on the host at which clients should connect.
     */
    void setSocket(std::string hostName, int portNum);
    
    /*!
     * A function that sends a message to the host. An error will be thrown if the socket is not set or if an error occurs in sending the message.
     *
     * @param message A std::string of the message to be sent.
     */
    void send(std::string message);
    
    /*!
     * A function that receives a message from the host. The function will wait for a short period for the client to send the message, and if the message is not received it will throw an error. An error is also thrown if the socket is not set.
     *
     * @return The received message from the host.
     */
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
    
    bool setUp = false; //Represents if the socket has already been set. If not, reading and writing will cause errors
};

#endif /* ClientSocket_hpp */
