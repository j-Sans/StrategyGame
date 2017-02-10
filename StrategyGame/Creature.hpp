//
//  Creature.hpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Creature_hpp
#define Creature_hpp

#include "Macros.h"

//Standard library includes
#include <string>
#include <queue>

//Local includes
#include "Attackable.hpp"

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

enum Race {
    Human, //Researching new units costs 33% less.
    Elf, //Can travel through forest as though it were open and gain a combat bonus while in forest.
    Dwarf, //Can enter mountains and has the mine ability while in mountain.
    Orc, //Units fight at full strength when damaged and gain half of missing health % as a combat bonus.
    Goblin, //Units cost 33% less.
    Undead, //Has a 50% change to convert defeated units into equal-strength undead
    Vampire, //Melee units gain 50% of damage dealt as life.
};

enum AttackStyle {
    Melee,
    Ranged,
    TerrainIgnoring, //ignores terrain (perhaps telepathic abilities)
};


class Creature : public Attackable {
public:
    //Constructors
    
    /*!
     * A class representing a creature object.
     *
     * @param x int x An unsigned int representing the x location of the creature on the board.
     * @param y int y An unsigned int representing the y location of the creature on the board.
     * @param race A Race enum type respresenting the creature's race. Includes: Human, Elf, Dwarf, Orc, Goblin, Undead, and Vampire.
     * @param maxHealth An unsigned int representing the max health of the creature.
     * @param maxEnergy An unsigned int representing the max energy of the creature.
     * @param attack An unsigned int representing the damage value the creature.
     * @param attackStyle An attackStyle enum type representing attack Style (projectile).
     * @param vision An unsigned int representing the radius of squares around the creature that it can reveal.
     * @param range An unsigned int representing the distance the creature can attack.
     * @param startDirection A macro representing the initial direction that the creature faces. Includes: NORTH, EAST, SOUTH, and WEST.
     * @param controller The player index representing the controller of the creature.
     */
    Creature(unsigned int x, unsigned int y, Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, AttackStyle attackStyle, unsigned int vision, unsigned int range, unsigned int startDirection, unsigned int controller);
    
    //Destructor
    
    //Public properties
    
    /*!
     * An std::queue of the directions that this creature has to take.
     * In the game class, the next direction will be popped off and the creature will go in that direction.
     */
    std::queue<int> directions;
    
    //Public member functions
    
    /*! 
     * Reduces the creature's health by the damage argument, and if the creature dies, returns true.
     *
     * @param damage An unsigned int representing damage this creature takes.
     *
     * @return True if the creature dies, otherwise false.
     */
    bool takeDamage(unsigned int damage);
    
    /*
     * Increases the creature's energy by the movement argument. Set to max if it would be greater.
     *
     * @param energy An int representing actions, including any movement or attacking that this creature does.
     */
    void incrementEnergy(int energy);
    
    /*
     * Reduces the creature's energy by the movement argument. Set to 0 if it would be set to less.
     *
     * @param energy An int representing actions, including any movement or attacking that this creature does. 
     */
    void decrementEnergy(int energy);
    
    /*!
     * Reduces the creature's energy to 0. To be used after combat or other abilities are used.
     */
    void useAllEnergy();
    
    /*!
     * Resets the creature's energy to its max energy value. To be used at the start of each turn.
     */
    void resetEnergy();
    
    /*!
     * Sets the direction of this creature, as long as the direction is valid. Includes NORTH, EAST, SOUTH, and WEST.
     */
    void setDirection(unsigned int direction);
    
    /*!
     * Increment the offset properly, based on the creature's direction.
     *
     * @param deltaTime The time between frames, so it's speed can be constant on all machines at different working speeds.
     *
     * @return Whether the creature reached the end destination.
     */
    bool incrementOffset(float deltaTime);
    
    /*!
     * Set the offset as -0.4 for moving down and increments it for moving up. This function should be called when the creature is starting to move.
     *
     * @param deltaTime The time between frames, so it's speed can be constant on all machines at different working speeds.
     */
    void initiateMovementOffset(float deltaTime);
    
    /*!
     * Reset the offset to 0.
     */
    void resetOffset();
    
    /*!
     * Update the x and y coordinates based on the direction.
     *
     * @param direction The direction in which to move.
     */
    void move(unsigned int direction);
    
    /*!
     * A function to update x and y based on parameters.
     *
     * @param x The new x coordinate.
     * @param y The new y coordinate.
     */
    void setLocation(unsigned int x, unsigned int y);
    
    //Get methods
    
    /*!
     * @return The race of this creature.
     */
    Race race();
    
    /*!
     * @return The race of this creature, in the form of an std::string.
     */
    std::string raceString();
    
    /*!
     * @return The maximum energy (which is the number of action points including movement) of this creature.
     */
    unsigned int maxEnergy();
    
    /*!
     * @return The attack stat of this creature.
     */
    unsigned int attack();
    
    /*!
     * @return The attack style of this creature.
     */
    AttackStyle attackStyle();

    /*!
     * @return The vision radius of this creature.
     */
    unsigned int vision();
    
    /*!
     * @return The attack range of this creature.
     */
    unsigned int range();
    
    /*!
     * @return The combat type (if it is melee versus ranged) of this creature.
     */
    const bool melee();
    
    /*!
     * @return The remaining action points of this creature.
     */
    float energy();
    
    /*!
     * @return The direction the creature is facing, as a macro (int). Includes: NORTH, EAST, SOUTH, and WEST.
     */
    int direction();
    
    /*!
     * @return The offset of the creature from the center of its tile.
     */
    float offset();
    
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
    static Creature deserialize(std::string str);
    
private:
    //Private properties
    
    //Stats
    Race raceVal;
    AttackStyle attackStyleVal;
    unsigned int maxEnergyVal;
    unsigned int attackVal;
    unsigned int visionVal;
    unsigned int rangeVal;
    
    unsigned int energyVal;
    int directionVal;
    float offsetVal = 0.0;
    
    //The speed that the creature moves when it is animated.
    constexpr static const float movementAnimationSpeed = 2.0f;
    
    //Private member functions
    
    /*!
     * A function to set the offset of this creature. Used within the deserialization function.
     *
     * @param offset A float representing the offset with which to override the previous value.
     */
    void setOffset(float offset);
};

#endif /* Creature_hpp */
