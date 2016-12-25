//
//  Receiver.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 12/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Receiver_hpp
#define Receiver_hpp

//Standard library includes
#include <string>

//Local includes
#include "Client.hpp"

class Receiver {
public:
    
    //Constructor
    Receiver(std::string hostName, int portNum);
    
    //Public member functions
    void render();
    void terminate();
    
    bool windowShouldClose();
    
private:
    
    //Socket, to receive information and give to the visualizer
    ClientSocket socket;
    
    //Client, to run the game
    Client client;
};

#endif /* Receiver_hpp */
