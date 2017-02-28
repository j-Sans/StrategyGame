//
//  Tile.hpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Tile_hpp
#define Tile_hpp

//Standard library includes
#include <iostream>
#include <queue>

//Local includes
#include "Macros.h"
#include "Creature.hpp"
#include "Building.hpp"

//GLM: OpenGL mathematics for rotations, translations, dilations
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#define OPEN_TERRAIN 0 //terrainMovementEnergyCosts[OPEN_TERRAIN] 1 
#define MOUNTAIN_TERRAIN 1
#define WATER_TERRAIN 2
#define FOREST_TERRAIN 3
#define HILL_TERRAIN 4
#define SWAMP_TERRAIN 5
#define ROAD_TERRAIN 6
//to add creature movement modifiers in terrain it needs to be set up as a creature stat.
//#define CARROT_TREE_TERRAIN 7 //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.

#define NO_CREATURE 0
#define STICK_FIGURE_CREATURE 1 //Simple test creature type using a stick-figure image
#define SCOUT_PLACEHOLDER 2 //Simple test creature type using an easily identifiable box texture
#define ARCHER_PLACEHOLDER 3 //Simple test creature type using an easily identifiable box texture

#define NO_BUILDING 0
#define TOWER_BUILDING 1 //Simple test building type using a tower image

class Tile {
public:
    //Constructors
    
    Tile(int terrain, unsigned int x, unsigned int y);

    
    //Public properties
    
    /*!
     * Time before deletion of damage boxes, in seconds.
     */
    constexpr static const float damageScreenTime = 4.0f;
    
    //Public member functions
    
    /*! 
     * Sets a creature as the creature located in this tile.
     * Warning: Does not deletes the old creature!
     *
     * @param creature A pointer to the new creature. If nullptr, then the spot becomes empty.
     */
    void setCreature(Creature *creature);
    
    /*!
     * Sets a building as the building located in this tile.
     * Warning: Does not deletes the old building!
     *
     * @param building A pointer to the new building. If nullptr, then the spot becomes empty.
     */
    void setBuilding(Building *building);
    
    /*!
     * Sets a direction at the creature at this tile.
     *
     * @param direction A direction macro representing the direction. Includes NORTH, EAST, SOUTH, and WEST.
     */
    void setDirection(unsigned int direction);
    
    /*!
     * Indicates the damage value, dealt to the creature or building at this tile, to be displayed.
     *
     * @param damage The amount of damage dealt.
     * @param time The current time, gotten from glfwGetTime().
     */
    void setDamage(unsigned int damage, float time);
    
    //Get methods
    
    /*!
     * @return The x position of the tile in the board.
     */
    unsigned int x() const;
    
    /*!
     * @return The y position of the tile in the board.
     */
    unsigned int y() const;
    
    /*!
     * @return A pointer to the creature at this tile.
     */
    Creature* creature() const;
    
    /*!
     * @return A pointer to the building at this tile.
     */
    Building* building() const;
    
    /*!
     * @return The terrain type of this tile.
     */
    unsigned int terrain() const;
    
    /*!
     * @return A boolean representing if there is a creature on this tile currently.
     */
    bool occupied() const;
    
    /*!
     * @return The type of the creature, indicating which texture to use to the openGL VBO. Currently returns the basic creature if there is any creature.
     */
    unsigned int creatureType() const;
    
    /*!
     * @return The type of the building, indicating which texture to use to the openGL VBO. Currently returns the basic building if there is any building.
     */
    unsigned int buildingType() const;
    
    /*!
     * @return A boolean representing whether the creature would be able to go through this tile.
     */
    bool passableByCreature(const Creature& creature) const;
    
    /*!
     * @return A boolean representing whether an attack style would be able to go through this tile. (arrows, fireballs, terrain-ignoring projectiles etc.)
     */
    bool passableByAttackStyle(const Creature& creature) const;
    
    /*!
     * @return A boolean representing whether a creature can see through this tile.
     */
    bool passableByVision(const Creature& creature) const;
    
    /*!
     * @return The damage amount at this tile.
     */
    unsigned int damage() const;
    
    /*!
     * @return The time the damage was added to this tile.
     */
    float timeOfDamage() const;
    
    /*!
     * Serialize this object as a string that can be sent through sockets.
     *
     * @return The serialized string.
     */
    std::string serialize() const;
    
    /*!
     * Create an object from a serialized string that can be sent through sockets.
     *
     * @param str The string to convert into an object.
     *
     * @return The object created.
     */
    static Tile deserialize(std::string str);
    
    //I think we should impliment these later because they may be complex to program without that much reward
    //const bool vision() { return this->blocksVision; }
    //const bool rough() { return this->isRoughTerrain; }
    
private:
    //Private properties
    
    //Location info
    const unsigned int tileX;
    const unsigned int tileY;
    
    //Creatures and buildings
    Creature* tileCreature = nullptr;
    Building* tileBuilding = nullptr;
    
    //Tile info
    int tileTerrain;
    
    //The damage to be shown on the creature at this tile.
    unsigned int tileDamage = 0;
    
    //The time the damage was first shown, so that after a given number of seconds (Tile::DamageScreenTime) the damage will no longer be displayed.
    float damageHitTime = 0;
    
    //Private member functions
};

#endif /* Tile_hpp */
