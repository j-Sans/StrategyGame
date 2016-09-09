//
//  Player.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 7/5/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <string>

#include "Macros.h"
#include "Game.hpp"

class Player {
public:
    Player(Board* gameBoard);
    
    Game game;
private:
    unsigned int mana;
};

#endif /* Player_hpp */
