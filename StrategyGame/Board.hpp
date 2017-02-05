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
#include <exception>

#include "Creature.hpp"
#include "Building.hpp"
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
    
    //Terrain Costs of Movement
    float getTerrainMovementCost(Tile origin, Tile destination);
    
    //Terrain Costs of Attack
    float getTerrainAttackCost (Tile origin, Tile destination);
    
    //Terrain Costs of Vision
    float getTerrainVisionCost (Tile origin, Tile destination);
    
    //Upkeep: Triggers every 10 seconds. Restores 1 energy to all creatures.
//    void upkeep();
    
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
    
    //Calculate missing HP debuff for combat
    float calculateWeaknessDebuff(Tile combatTile);
    
    //Calculate flanking bonus for combat
    float calculateFlankingBonus(Tile attacker, Tile defender);
    
    //Calculate terrain modifier for combat
    float calculateTerrainModifier(Tile defender);
    
    /*!
     * Have one creature attack the other creature. Possible errors include if the indices are off of the board.
     
     * @param attackerX The x index of the attacker in the board.
     * @param attackerY The y index of the attacker in the board.
     * @param defenderX The x index of the defender in the board.
     * @param defenderY The y index of the ∂efender in the board.
     * @param attackDamage An int to store the damage dealt by the attacker. Use nullptr if no int is needed to store this.
     * @param defendDamage An int to store the damage dealt by the defender. Use nullptr if no int is needed to store this.
     *
     * @return A std::vector of std::pairs of std::strings and ints, representing the action strings and the respective controller, that should be done upon death of any of the creatures or buildings.
     */
    std::vector<std::pair<std::string, int> > initiateCombat(unsigned int attackerX, unsigned int attackerY, unsigned int defenderX, unsigned int defenderY, int* attackDamage, int* defendDamage);
    
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
     * Resets all energy values of creatures controlled by a certain player.
     *
     * @param The number associated with the player whose creatures to update.
     */
    void resetEnergy(unsigned int player);
    
    /*!
     * Set a creature in the designated spot on the board.
     * Possible errors include if the coordinates that are part of the creature are outside of the range of the board.
     *
     * @param creature A creature object to be set in the board at the location based on its coordinates.
     */
    void setCreature(Creature creature);
    
    /*!
     * Delete the creature in the designated spot on the board.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     *
     * @return An std::string representing the action to do on when the creature dies.
     */
    std::string deleteCreature(unsigned int x, unsigned int y);
    
    /*!
     * Set a building in the designated spot on the board.
     * Possible errors include if the coordinates that are part of the creature are outside of the range of the board.
     *
     * @param creature A creature object to be set in the board at the location based on its coordinates.
     */
    void setBuilding(Building building);
    
    /*!
     * Delete the building in the designated spot on the board.
     * Possible errors include if the coordinates are outside of the range of the board.
     *
     * @param x The x index of the coordinate in the board.
     * @param y The y index of the coordinate in the board.
     *
     * @return An std::string representing the action to do on when the creature dies.
     */
    std::string deleteBuilding(unsigned int x, unsigned int y);
    
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
     * A function to check the validity of tile coordinates.
     *
     * @param tilePos The coordinates of the tile to check.
     *
     * @return True if the given location exists on the board, false otherwise.
     */
    bool validTile(glm::ivec2 tilePos);
    
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
    
    /*!
     * A function to check if a creature can be moved to the destination from its current location. An invalid_argument exception will be thrown if there is no creature, and a range_error if either set of coordinates is not on the board.
     *
     * @param destination A glm::ivec2 representing the board coordinates of the destination tile.
     * @param currentLoc A glm::ivec2 representing the board coordinates of the creature's current location.
     *
     * @return Whether the creature is able to move there.
     */
    bool destinationInRange(glm::ivec2 destination, glm::ivec2 currentLoc);
    
    /*!
     * A function to check if a creature can attack the destination from its current location. An invalid_argument exception will be thrown if there is no creature at either destination or the current location, a range_error if either set of coordinates is not on the board, and a logic_error if the creature has no energy.
     *
     * @param destination A glm::ivec2 representing the board coordinates of the defender's tile.
     * @param currentLoc A glm::ivec2 representing the board coordinates of the creature's current location.
     *
     * @return Whether the creature is able to attack.
     */
    bool attackInRange(glm::ivec2 destination, glm::ivec2 currentLoc);
    
    /*!
     * A function that gets all of the tiles that a creature can reach.
     *
     * @param creature The tile containing the specified creature that is going to be moving.
     *
     * @return An std::vector of Tile objects containing all of the tiles that can be reached.
     */
    std::vector<Tile> getReachableTiles(Tile creatureTile);
    
    /*!
     * A function that gets all of the tiles that a creature can attack.
     *
     * @param creature The tile containing the specified creature that is going to be moving.
     *
     * @return An std::vector of Tile objects containing all of the tiles that can be attacked.
     */
    std::vector<Tile> getAttackableTiles(Tile creatureTile);
    
    /*!
     * A function that gets all of the tiles that a creature can see.
     *
     * @param creature The tile containing the specified creature that is seeing.
     *
     * @return An std::vector of Tile objects containing all of the tiles that can be seen.
     */
    
    std::vector<Tile> getVisibleTiles(Tile creatureTile);
    
    /*!
     * A function that gets all of the tiles that a player can see
     *
     * @return An std::vector of Tile objects containing all of the tiles that can be seen.
     */
    std::vector<Tile> getAllVisibleTiles();
    
    /*!
     * Serialize this object as a string that can be sent through sockets.
     *
     * @return The serialized string.
     */
    std::string serialize();
    
    /*!
     * Create an object from a serialized string that can be sent through sockets.
     *
     * @param str The string to convert into an object.
     *
     * @return The object created.
     */
    static Board deserialize(std::string str);
    
private:
    //Private properties
    std::vector<std::vector<Tile> > gameBoard;
    std::list<Creature> creatures; //List of creatures on the game board, for board tiles to have pointers to
    std::list<Building> buildings; //List of buildings on the game board, for board tiles to have pointers to
    
    //Private member functions
};

#endif /* Board_hpp */
