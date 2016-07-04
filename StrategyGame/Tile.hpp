//
//  Tile.hpp
//  Game
//
//  Created by Jake Sanders on 6/22/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#ifndef Tile_hpp
#define Tile_hpp

#include "Creature.hpp"
#include <iostream>

//GLM: OpenGL mathematics for rotations, translations, dilations
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#define OPEN_TERRAIN 0
#define MOUNTAIN_TERRAIN 1
#define WATER_TERRAIN 2
#define CARROT_FARM_TERRAIN 3 //When Truell someday looks through the codebase, he will find this edit, and he will be satisfied.

#define NO_CREATURE 0
#define STICK_FIGURE_CREATURE 1 //Simple test creature type using a stick-figure image

enum Color {
    White,
    Grey,
    Red,
    Yellow,
    Green,
    Cyan,
    Blue,
};

enum Style {
    Regular,
    Selected,
    OpenAdj,
    AttackableAdj,
};

/**
 * A class representing a single tile on the board and, if present, the creature on that tile.
 *
 * @param terrain A macro representing the terrain type. Possible options include: OPEN_TERRAIN, MOUNTAIN_TERRAIN, WATER_TERRAIN, and more to be added.
 * @param x The x coordinate of this tile on the board.
 * @param y The y coordinate of this tile on the board.
 */
class Tile {
public:
    //Constructors
    Tile(float terrain, unsigned int x, unsigned int y);
    
    //Destructor
    
    //Public properties
    
    /**
     * The color of selected tiles.
     */
    static const Color selectedTileColor = Grey;
    
    /**
     * The color of adjacent open tiles.
     */
    static const Color openAdjTileColor = Cyan;
    
    /**
     * The color of adjacent attackable tiles.
     */
    static const Color attackableAdjTileColor = Red;
    
    //Public member functions
    
    /** 
     * Sets a creature as the creature located in this tile.
     *  Warning: Does not deletes the old creature!
     *
     * @param creature A pointer to the new creature. If nullptr, then the spot becomes empty.
     */
    void setCreature(Creature *creature);
    
    /**
     * Sets a style to alter the hue of this tile.
     *
     * @param style A style type representing the color. Styles include Regular, Selected, OpenAdj, and AttackableAdj.
     */
    void setStyle(Style style);
    
    /**
     * Sets a direction at the creature at this tile.
     *
     * @param direction A direction macro representing the direction. Includes NORTH, EAST, SOUTH, and WEST.
     */
    void setDirection(unsigned int direction);
    
    //Get methods
    
    /**
     * @return The x position of the tile in the board.
     */
    const unsigned int x();
    
    /**
     * @return The y position of the tile in the board.
     */
    const unsigned int y();
    
    /**
     * @return A pointer to the creature at this tile.
     */
    Creature* creature();
    
    /**
     * @return The terrain type of this tile.
     */
    float terrain();
    
    /**
     * @return A boolean representing if there is a creature on this tile currently.
     */
    bool occupied();
    
    /**
     * @return The type of the creature, indicating which texture to use to the openGL VBO. Currently returns the basic creature if there is any creature.
     */
    unsigned int creatureType();
    
    /**
     * @return The name of the color of this tile, of type Color enum. Possible values include White, Grey, Red, Yellow, Green, Cyan, and Blue.
     */
    Color colorName();
    
    /**
     * @return The color alteration of the tile as a vec3 of RGB values. Values range from 0.0 to 1.0.
     */
    glm::vec3 color();
    
    /**
     * @return The style of this tile, of type Style enum. Possible values include Regular, Selected, OpenAdj, and AttackableAdj.
     */
    Style style();
    
    /**
     * @return A boolean representing whether the creature would be able to go through this tile.
     */
    bool passableByCreature(Creature creature);
    
    //I think we should impliment these later because they may be complex to program without that much reward
    //const bool vision() { return this->blocksVision; }
    //const bool rough() { return this->isRoughTerrain; }
    
private:
    //Private properties
    const unsigned int tileX;
    const unsigned int tileY;
    Creature* tileCreature = nullptr;
    int tileTerrain;
    Color tileColor = White;
    Style tileStyle = Regular;
    
    //const bool blocksVision;
    //const bool isRoughTerrain; //slows movement
    
    //Private member functions
};

#endif /* Tile_hpp */
