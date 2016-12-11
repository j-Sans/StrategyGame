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

#include <string>
#include <queue>

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
    LightRanged,      //cannot shoot through forest, mountain, hill
    HeavyMelee, //traditional large weapon melee (battleaxes, broadswords, etc.) can't be used in forests
    LightMelee, //usable in confined spaces (knives, machetes, etc.)
    HeavyRanged, //can travel through 1 forest by destroying it. Otherwise same as arrow
    TerrainIgnoring, //ignores terrain (perhaps telepathic abilities)
};

enum Name {
    //Military - unit research possibilities
    
        //Military Line (Melee) ---------------------------------------------------------------------------------------------------------------------------
        Recruit, //Basic scrub, holds his weapon backwards. Can Upgrade To: Thief, Scrapper,   Swordsman, Pikeman, Clubman, Arsonist,    Horseman.
    
            //Light Melee Covert-------------------------------------
            Thief, //Steals money from defeated units
    
                //Fast,
                Stalker,
                    Assassin, //Stronger when facing isolated units. Poisons targets.
                    Ninja,  //Very Fast
                    Bandit, //Steals money from defeated units.

                //Interferes with enemy operations
                Spy, //extra vision
                    Traitor,   //EXTREMELY DIFFICULT: All players see this unit as their own color.
                        Skinner, //Can become invisible to enemies for a few turns at the cost of energy. Has an ability which consumes it and takes control of an enemy minion.
                    Saboteur,  //Does huge damage to enemy buildings
                    Agent, //Can become invisible to enemies for a few turns at the cost of energy. Extra vision.
                        JamesBond,
    
            //Light Melee Fighters-------------------------------------
            Scrapper, //A street fighter
                HandAxe, //Melee unit that fights with axes
                    AxeThrower, //A 1-ranged unit with the strength of a melee unit.
    
                Bladewhirl, //Melee unit that fights with dual knives
                    DervishBlade, //A whirlwind of steel
                    PoisonKnife, //Poisons his knives.
    
                Forester,
                    SpiritOfTheForest, //Heals each turn when in Forest.
                    VerdantProtector, //LightMelee. Much stronger in Forest.
    
            //Heavy Melee Line-----------------------------------------
            Swordsman, //Standard swordsman
    
                Knight,
                    TemplarPaladin, //Heals himself each turn,
                    GrayWarden, //Drains the health of surrounding foes,
                    Defender, //Stronger on Defense, Weaker on Offense.
    
    
                Duelist, //Becomes stronger after killing units.
                    GrandChampion, //Becomes stronger and heals after killing units
                    CombatExpert, //Becomes stronger after killing units and has a 50% dodge chance. //Gets huge bonus against units he has killed before.
    
            Pikeman, //Anti-Cavalry infantry unit. +50%
                Halberdier, //Anti-Cavalry infantry unit. +100%
    
            Clubman,
                Berserker, //Gains missing health as combat strength
                    VikingFury, //Gains missing health as combat strength and cannot be reduced below 1 HP for 3 turns upon taking lethal damage.
    
                Maceman, //Deals 50% additional damage to his target.
                    Crusher, //Deals double damage to his target.
                MorningStar, //Deals 25% of damage dealt to units adjacent to his target.
                    Devastator, //Does 50% damage to units adjacent to his target as well.
    
            Arsonist, //Sets targets on fire. Extra effective against buildings.
                Flamethrower, //Weak, but super-effective against units and sets targets on fire. Hits the square behind his target.
                BombLobber, //Super-effective against buildings. Hits tiles adjacent to target for 20%.
                LooseCannon, //Has a 50% chance to miss. Deals massive damage.
                Experimenter, //Weak Unit. May become stronger with investment. This line may not be implemented.
                    Handcannon,
                        Pistolero,
                            Militia,
                                Musketeer,
                                    Rifleman,
                                        Gunman,
                                            Sniper,
                                            SpecialForces,
                                            HeavyWeapons,
                                                //Need all three of these techs to unlock:
                                                XCOMSQUAD, //infinite movement.
    
            //Cavalry Division (Recruit becomes Horseman)
            Horseman, //4 Energy.
                //Light Melee Cavalry-------------------------------------
                LightCavalry, //A faster Horseman. 6 Energy.
                    RoyalHussar, //Flanking attack bonus is increased to 20% from 10%
                    Cavalryman, //Stronger and faster than a horseman
    
                //Heavy Melee Cavalry-------------------------------------
                HeavyCavalry, //A stronger Horseman. 4 Energy.
                    Cataphract, //Deals a ranged attack to the square it is facing at the end of your turn.
                        ElephantRider, //Deals a ranged attack to the square it is facing at the end of your turn.

                    Cavalier, //Does +100% damage if it has traveled at least 4 unique squares this turn when attacking.
                        HeavyBrigade, //Instantly kills the target if it has traveled at least 3 unique squares this turn when attacking.
    
        //Military Line (Ranged) --------------------------------------------------------------------------------------------------------------------------
        Slinger,  //Slings stones. Can Upgrade To: Hunter, Archer.
    
            //Light Ranged Line
            Hunter,
                //Stronger in Forest
                Yeoman,
                    ForestGuardian,
                    ForestRanger,
    
                Blowgunner, //Poisons enemies upon damage.
                    TribalCommander, //Poisons enemies upon damage. Stronger against Poisoned targets.
    
            //Heavy Ranged Line
            Archer,
                //Greater Power
                Crossbowman,
                    ImperialArbalest,
    
                //Greater Range
                Longbowman,
                    EagleEye, //Has longest range in game. Shoots fire arrows.
    
            //Mounted Ranged Line (Count as LightRanged)
            Nomad,
                Keshik, //Promotes 50% Faster, Can Move After Attacking. Higher defense against ranged attacks.
                    Mangudai,   //Promotes 50% Faster, Can Move After Attacking. Higher defense against ranged attacks. Returns fire on ranged attacks.
                    SteppeRider,    //Promotes 50% Faster, Can Move After Attacking. Higher defense against ranged attacks. Stronger in Hills.
                CavalryArcher, //Stronger attack and defense. Higher defense against melee.
    
    
    
        //Support Line (No Attack) --------------------------------------------------------------------------------------------------------------------------
            //The units following each category are simply defensive buffs to scale with later-game units. They provide the same function adjusted for lategame.
        Worker, //Builds battlefield improvements and fortifications.
            Engineer,
                CombatEngineer,
        Healer, //Heals adjacent units at the end of each turn.
            Medic,
                Surgeon,
        Planner, //Buffs nearby Units.
            Tactician,
                Commander,
        Scout,  //Grants map vision and information through quick movement and good eyesight.
            Lookout,
    
    //Wizards
    
    
    //Beasts
    Lizard,
        Dragon,
    
    Bird,
        Phoenix,
        Thunderbird,
        Roc,
    
    Wolf,
        Werewolf,
    
    Fish,
    
    //!!?!??!?!?!?!?!?!?! CREATURES FROM THE BEYOND HAVE RAMPAGED INTO THIS WORLD
};


class Creature {
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
     * @param cost An unsigned int representing the cost of the creature in mana.
     * @param startDirection A macro representing the initial direction that the creature faces. Includes: NORTH, EAST, SOUTH, and WEST.
     */
    Creature(unsigned int x, unsigned int y, Race race, unsigned int maxHealth, unsigned int maxEnergy, unsigned int attack, AttackStyle attackStyle, unsigned int vision, unsigned int range, unsigned int cost, unsigned int startDirection, unsigned int controller);
    
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
    
    /*!
     * Reduces the creature's energy by the movement argument.
     *
     * @param energy An unsigned int representing movement this creature does. Equal to the energy loss.
     */
    void decrementEnergy(unsigned int energy);
    
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
    const Race race();
    
    /*!
     * @return The race of this creature, in the form of an std::string.
     */
    std::string raceString();
    
    /*!
     * @return The maximum health of this creature.
     */
    const unsigned int maxHealth();
    
    /*!
     * @return The maximum energy (which is the number of action points including movement) of this creature.
     */
    const unsigned int maxEnergy();
    
    /*!
     * @return The attack stat of this creature.
     */
    const unsigned int attack();
    
    /*!
     * @return The attack style of this creature.
     */
    const AttackStyle attackStyle();


    /*!
     * @return The vision radius of this creature.
     */
    const unsigned int vision();
    
    /*!
     * @return The attack range of this creature.
     */
    const unsigned int range();
    
    /*!
     * @return The purchase cost of this creature.
     */
    const unsigned int cost();
    
    /*!
     * @return The combat type (if it is melee versus ranged) of this creature.
     */
    const bool melee();
    
    /*!
     * @return The remaining health of this creature.
     */
    unsigned int health();
    
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
     * @return The x location of this creature on the board.
     */
    unsigned int x();
    
    /*!
     * @return The y location of this creature on the board.
     */
    unsigned int y();
    
    /*!
     * @return An unsigned int representing the controller of the creature.
     */
    const unsigned int controller();
    
private:
    //Private properties
    const unsigned int creatureController;
    
    //Stats
    const Race creatureRace;
    const AttackStyle creatureAttackStyle;
    const unsigned int creatureMaxHealth;
    const unsigned int creatureMaxEnergy;
    const unsigned int creatureAttack;
    const unsigned int creatureVision;
    const unsigned int creatureRange;
    const unsigned int creatureCost;
    
    unsigned int creatureHealth;
    unsigned int creatureEnergy;
    int creatureDirection;
    float creatureOffset = 0.0;
    
    //Location
    unsigned int creatureX;
    unsigned int creatureY;
    
    //The speed that the creature moves when it is animated.
    constexpr static const float movementAnimationSpeed = 2.0f;
    
    //Private member functions
};

#endif /* Creature_hpp */
