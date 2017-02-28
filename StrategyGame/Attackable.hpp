//
//  Attackable.hpp
//  Strategy Game
//
//  Created by Jake Sanders on 1/28/17.
//  Copyright © 2017 Jake Sanders. All rights reserved.
//

#ifndef Attackable_hpp
#define Attackable_hpp

#include <list>
#include <string>

class Attackable {
public:
    
    //Constructor
    Attackable(unsigned int maxHealth, unsigned int x, unsigned int y, unsigned int controller);
    
    //Public properties
    std::string deathAction = ""; //Initially nothing, can be modified
    
    //Public member functions
    
    /*!
     * Adds a button to be displayed when this object is selected.
     *
     * @param action The action to do when the button is selected.
     * @param text The text that the button will display.
     */
    void addButton(std::string action, std::string text);
    
    /*!
     * Removes all button with the given action.
     *
     * @param action The std::string of the action to look for.
     */
    void removeButtonByAction(std::string action);
    
    /*!
     * Removes all button with the given action.
     *
     * @param text The std::string of the text to look for.
     */
    void removeButtonByText(std::string text);
    
    /*!
     * Removes the first button added.
     */
    void removeFirstButton();
    
    /*!
     * Removes the most recently added button.
     */
    void removeLastButton();
    
    /*!
     * @return The maximum health of this object.
     */
    const unsigned int maxHealth();
    
    /*!
     * @return The current health of this object.
     */
    const unsigned int health();
    
    /*!
     * @return The x location of this object on the board.
     */
    const unsigned int x();
    
    /*!
     * @return The y location of this object on the board.
     */
    const unsigned int y();
    
    /*!
     * @return An unsigned int representing the controller of the object.
     */
    const unsigned int controller();
    
    /*!
     * Serialize the buttons of this object as a string that can be sent through sockets.
     *
     * @return The serialized string.
     */
    const std::string serialize();
    
    /*!
     * Get information about the buttons from a serialized string that can be sent through sockets.
     *
     * @param str The string to convert into an object.
     *
     * @return A vector containing the button info.
     */
    static std::list<std::pair<std::string, std::string> > deserializeButtons(std::string str);
    
protected:
    //Protected properties
    
    unsigned int maxHealthVal;
    unsigned int controllerVal;
    
    //Location
    unsigned int xVal;
    unsigned int yVal;
    
    unsigned int healthVal;
    
    //Contains information on the buttons that will be offered when this object is selected
    std::list<std::pair<std::string, std::string> >buttonInfo; //list of <action, text>
};

#endif /* Attackable_hpp */
