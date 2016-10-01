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
#include "Visualizer.hpp"

class Client {
public:
    
    //Constructor
    Client(std::string hostName, int portNum);
    
    render(std::map<BoardInfoDataTypes, std::string> boardInfo);
    
private:
    
    //Socket
    ClientSocket socket;
};

#endif /* Client_hpp */
