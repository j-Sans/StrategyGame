//
//  Client.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 10/1/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

//Standard library includes
#include <string>
#include <map>

//Local includes
#include "Board.hpp"
#include "Visualizer.hpp"

class Client {
public:
    
    //Constructor
    Client(std::string hostName, int portNum);
    
    //Public member functions
    
    void render();
    void terminate();
    
    bool windowShouldClose();
    
private:
    //Private properties
    
    //Board info
    int boardWidth;
    int boardHeight;
    Board board;
    
    //Info unique to clinet
    unsigned int playerNum;
    std::vector<std::vector<float> > offsets;
    
    
    //Visualizer, to render the window
    Visualizer visualizer;
    
    //Private member functions
    void updateCreatures(float deltaTime);
};

#endif /* Client_hpp */
