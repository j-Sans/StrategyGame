//
//  Host.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 9/8/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Host_hpp
#define Host_hpp

//Standard library includes
#include <vector>
#include <exception>

//Local includes
#include "ServerSocket.hpp"
#include "Player.hpp"

class Host {
public:
    
    //Constructor
    Host(unsigned int numberOfPlayers, int portNum);
    
private:
    
    //Private properties
    
    ServerSocket socket;
    
    std::vector<Player> players;
    
};

#endif /* Host_hpp */
