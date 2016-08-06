//
//  Board.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 6/26/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include "Macros.h"

#include <math.h>
#include <vector>
#include <list>
#include <experimental/optional>
#include <exception>

#include "Tile.hpp"

//How creatures are stored:
//Creatures will be put into a std::list and then tiles will have pointers to the list elements. Each creature will have a unique position on the board, so the x and y coordinates are a unique combination for each creature, and can be used as a identifier for finding and deleting that creature.

class Board {
public:
    //Constructor
    
    /*!
     * A board class representing a 2D vector of the board.
     *
     * @param board A 2D vector of Tiles representing the game board.
     */
    Board(std::vector<std::vector<Tile> > board);
    
    //Destructor
    
    //Public properties
    
    //Public member functions
    
    /*!
     * Move a creature from the designated spot in the designated direction.
     * Possible errors include if the destination is off of the board.
     
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     * @param direction The direction in which to move. Possible direction types include NORTH, EAST, SOUTH, and WEST. If the direction is not valid nothing happens.
     
     * @return Whether the creature could be moved in the direction specified.
     */
    bool moveCreatureByDirection(unsigned int x, unsigned int y, unsigned int direction);
    
    /*!
     * Move a creature from the designated spot in the designated direction.
     * Possible errors include if the destination is off of the board.
     
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     * @param destinationX The x index of the destination coordinate in the board.
     * @param destinationY The y index of the destination coordinate in the board.
     
     * @return Whether the creature was moved to the destination. False is returned if it is already at its destination.
     */
    bool moveCreatureByLocation(unsigned int x, unsigned int y, unsigned int destinationX, unsigned int destinationY);
    
    /*!
     * Have one creature attack the other creature. Possible errors include if the indices are off of the board.
     
     * @param attackerX The x index of the attacker in the board.
     * @param attackerY The y index of the attacker in the board.
     * @param defenderX The x index of the defender in the board.
     * @param defenderY The y index of the ∂efender in the board.
     * @param attackDamage An int to store the damage dealt by the attacker. Use nullptr if no int is needed to store this.
     * @param defendDamage An int to store the damage dealt by the defender. Use nullptr if no int is needed to store this.
     
     * @return Whether combat occurred or not, for various reasons.
     */
    bool attack(unsigned int attackerX, unsigned int attackerY, unsigned int defenderX, unsigned int defenderY, int* attackDamage, int* defendDamage);
    
    /*!
     * Get the distance (in taxicab geometry) from one tile to another.
     * Possible errors include if the coordinates are off of the board.
     *
     * @param x1 The x index of the first coordinate in the board.
     * @param y1 The y index of the first coordinate in the board.
     * @param x2 The x index of the second coordinate in the board.
     * @param y2 The y index of the second coordinate in the board.
     *
     * @return The distance (in taxicab geometry) from one tile to another.
     */
    unsigned int tileDistances(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
    
    /*!
     * Set a creature in the designated spot on the board.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     * @param creature A creature object to be set in the board at the designated spot.
     */
    void setCreature(unsigned int x, unsigned int y, Creature creature);
    
    /*!
     * Delete the creature in the designated spot on the board.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     */
    void deleteCreature(unsigned int x, unsigned int y);
    
    /*!
     * Set a color in the designated spot on the board.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     * @param style A style to be set in the board at the designated spot. Possible colors include Regular, Selected, OpenAdj, and AttackableAdj.
     */
    void setStyle(unsigned int x, unsigned int y, Style style);
    
    /*!
     * Set a direction for the creature at the designated spot on the board.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     * @param direction A direction macro to be set at the creature at the designated spot. Includes: North, East, South, and West.
     *
     * @return whether a direction was successfully set for a creature at the designated location.
     */
    bool setDirection(unsigned int x, unsigned int y, unsigned int direction);
    
    /*!
     * Indicates the damage value, dealt to the creature at this tile, to be displayed.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     * @param damage The amount of damage dealt.
     * @param time The current time, gotten from glfwGetTime().
     */
    void setDamage(unsigned int x, unsigned int y, unsigned int damage, float time);
    
    //Public get functions
    
    /*!
     * Get the tile at the spot (x,y) from the board.
     * Possible errors include if the x or y coordinates are greater than or equal to the size of the respective vectors.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     *
     * @return The tile object at this location in the board.
     */
    Tile get(unsigned int x, unsigned int y);
    
    /*!
     * @return The x-side width of the board.
     */
    unsigned int width();
    
    /*!
     * @param x The column x coordinate from which to get the size.
     *
     * @return The y-side height of the board.
     */
    unsigned int height(unsigned int x);
    
private:
    //Private properties
    std::vector<std::vector<Tile> > gameBoard;
    std::list<Creature> creatures; //List of creatures on the game board, for board tiles to have pointers to
    std::list<Building> buildings; //List of buildings on the game board, for board tiles to have pointers to
    
    //Private member functions
};

#endif /* Board_hpp */
