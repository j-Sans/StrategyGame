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

class ServerSocket {
public:
    //Constructor
    ServerSocket();
    
    //Destructor
    ~ServerSocket();
    
    //Public properties
    
    //Public member functions
    
    /*!
     * A function to initialize the socket. This must be done before the socket can be used. Will throw an error if the socket cannot be opened or if the port is occupied.
     *
     * @param portNum The number of the port on the host at which clients should connect.
     */
    void setSocket(int portNum);
    
    /*!
     * A function that adds a client. If there is no client, then the function waits for a connectino to be initiated by the client. Will throw an error if the maximum number of sockets (see MAXIMUM_NUMBER_OF_SOCKETS) have already been set, or if an error occurs connecting to the client.
     */
    void addClient();
    
    /*!
     * A function that sends a message to a single client. An error will be thrown if the socket is not set, if the given index is out of range, or if an error occurs in sending the message.
     *
     * @param message A std::string of the message to be sent.
     * @param clientIndex An unsigned int indicating the index of the client to whom to send the message.
     */
    void send(std::string message, unsigned int clientIndex);
    
    /*!
     * A function that sends a message to all clients. An error will be thrown if the socket is not set or if an error occurs in sending the message to any of the clients.
     *
     * @param message A std::string of the message to be sent.
     */
    void broadcast(std::string message);
    
    /*!
     * A function that receives a message from a single client. The function will wait for a short period for the client to send the message, and if the message is not received it will throw an error. An error is also thrown if the index is out of range or if the socket is not set.
     *
     * @param clientIndex An unsigned int indicating the index of the client from whom to receive the message.
     *
     * @return The received message from the specified client.
     */
    std::string receive(unsigned int clientIndex);
    
    /*!
     * A function that checks if all clients sent a specific message. This function calls ServerSocket::receive() so if another message has been sent that message may be received instead. This function throws no errors other than those called by ServerSocket::receive().
     *
     * @param messageToCompare The message that is checked with all clients.
     *
     * @return True if all clients sent the same message as the one passed in. False otherwise.
     */
    bool allReceived(std::string messageToCompare);
    
    /*!
     * @return The number of clients of this socket.
     */
    unsigned int numberOfClients();
    
    /*!
     * @return If this object is set.
     */
    bool getSet();
    
private:
    //Private properties
    unsigned int connectedClients = 0;
    
    //These are "file descriptors", which store values from both the socket system call and the accept system call
    int hostSocket;
    int clientSockets[MAX_NUMBER_OF_CONNECTIONS];
    int portNumber; //The port nubmer where connections are accepted
    socklen_t clientAddressSizes[MAX_NUMBER_OF_CONNECTIONS]; //The size of the address of the client, for the accept system call
    
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
    sockaddr_in clientAddresses[MAX_NUMBER_OF_CONNECTIONS];
    
    bool setUp = false; //Represents if the socket has already been set. If not, reading and writing will cause errors
    
};

#endif /* ServerSocket_hpp */
