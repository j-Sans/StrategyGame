//
//  main.cpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

//C library includes
#include <stdlib.h>
#include <time.h>

//Game includes
#include "Tile.hpp"

int main(int argc, const char * argv[]) {
    //Set up
    srand((int)time(NULL));
    
    //Set to true when the game should end
    bool gameOver = false;
    
    Tile* gameBoard[10][10];
    
    //Game loop
    while(!gameOver) {
        
    }
    
    return 0;
}
