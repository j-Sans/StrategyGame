//
//  Host.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 9/8/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Host.hpp"

Host::Host(unsigned int numberOfPlayers, int portNum, Board gameBoard) : board(gameBoard), socket(ServerSocket()) {
    this->socket.setSocket(portNum);
    for (int a = 0; a < numberOfPlayers; a++) {
        this->socket.addClient();
    }
    
    //Send to each player that player's number and the board
    for (int a = 0; a < numberOfPlayers; a++) {
        this->socket.send(std::to_string(a) + "," + this->board.serialize(), a);
    }
    if (!this->socket.allReceived("initialDataReceived"))
        throw std::runtime_error("Initial data not received");
    
    //Initialize each player with the board and that player's number
    int playerNum = 0;
    while (this->players.size() < numberOfPlayers) {
        this->players.push_back(Player(&board, playerNum++));
    }
    
    //Initialize time
    this->programStartTime = std::chrono::steady_clock::now();
    this->lastFrame = std::chrono::steady_clock::now() - this->programStartTime;
}

std::string Host::Host::storeVectorOfInts(std::vector<int> vec) {
    std::string str;
    
    for (int a = 0; a < vec.size(); a++) {
        str += std::to_string(vec[a]) + ',';
    }
    
    return str;
}

std::string Host::Host::storeVectorOfFloats(std::vector<float> vec) {
    std::string str;
    
    for (int a = 0; a < vec.size(); a++) {
        str += std::to_string(vec[a]) + ',';
    }
    
    return str;
}

void Host::update() {
    //Update frame information first
    std::chrono::duration<float> currentFrame = std::chrono::steady_clock::now() - this->programStartTime;
    this->deltaTime = currentFrame.count() - this->lastFrame.count();
    
    //Go through all tiles' damage and reset them if enough time has passed
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            if (this->board.get(x, y).damage() > 0 && currentFrame.count() - this->board.get(x, y).timeOfDamage() > Tile::damageScreenTime)
                this->board.setDamage(x, y, 0, currentFrame.count());
        }
    }
    
    for (int a = 0; a < this->players.size(); a++) {
        this->players[a].updateCreatures(this->deltaTime);
    }
    
    this->socket.broadcast(this->board.serialize());

    for (int a = 0; a < this->socket.numberOfClients(); a++) {
        std::string clientInfo = this->socket.receive(a);
        while (clientInfo.size() > 0) {
            this->processAction(clientInfo.substr(0, clientInfo.find_first_of(';')), a); //Process the action
    
            clientInfo = clientInfo.substr(clientInfo.find_first_of(';') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
        }
    }

//    this->socket.broadcast(std::to_string(this->activePlayer));
//    if (!this->socket.allReceived("activePlayerReceived"))
//        throw std::runtime_error("Active player not received");
//
//    /* Each index of clientInfo stores information:
//     1) Current mouse position's x coordinate on the board.
//     2) Current mouse position's y coordinate on the board.
//            -1 for either (1) or (2) represents NO_SELECTION
//     3) Whether the mouse is down or not. '0' represents up (false), and '1' represents down (true).
//     
//     Any other input should be done here. That hasn't been implimented.
//     */
//    
//    for (int a = 0; a < clientInfo.size(); a++) {
//        glm::ivec2 selectedTile;
//        
//        selectedTile.x = std::stoi(clientInfo[a].substr(0, clientInfo[a].find_first_of(','))); //Convert the substring to an int
//        
//        clientInfo[a] = clientInfo[a].substr(clientInfo[a].find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
//        
//        selectedTile.y = std::stoi(clientInfo[a].substr(0, clientInfo[a].find_first_of(','))); //Convert the substring to an int
//        
//        clientInfo[a] = clientInfo[a].substr(clientInfo[a].find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
//        
//        if (selectedTile.x < 0 || selectedTile.y < 0) {
//            selectedTile = NO_SELECTION;
//        }
//        
//        bool mouseDown = (clientInfo[a][0] - 48 == 0 ? false : true); //The first character in the string should be whether the mouse is up or down because the two parts before it, the mouse's x and y locations, were extracted and removed
//        
//        //Now extract the actions sent from the client
//        clientInfo[a].erase(0, 1); //Set the string to the contain only the list of actions, starting with the first action string. This erases the preceeding ';'
//        
//        while (clientInfo[a].size() > 1) { //If there is more than just the semicolon at the end of the last
//            this->processAction(clientInfo[a].substr(clientInfo[a].find_first_of(';') + 1), a); //Process the next action string
//            clientInfo[a] = clientInfo[a].substr(clientInfo[a].find_first_of(';') + 1, std::string::npos); //Delete the processed action
//        }
//        
//        this->players[a].updateSelected(mouseDown, selectedTile, currentFrame.count());
//        
//        this->players[a].updateCreatures(this->deltaTime);
//    }
//
//    this->socket.broadcast("End of frame");
    
    //Set the tiem of the previous frame to currentTime
    this->lastFrame = currentFrame;
}

std::string Host::serialize() {
    std::string str = "Host:" + this->board.serialize() + "," + std::to_string(this->deltaTime) + ",";
//    for (int a = 0; a << this->players.size(); a++) {
//        str += this->players[a].serialize() + ",";
//    }
    return str;
}

//Private member functions

void Host::processAction(std::string action, int playerNum) {
    if (action.find("move_creature_at_") != std::string::npos) {
        //Parse the destination tile from the string
        glm::ivec2 destination;
        destination.x = std::stoi(action.substr(0, action.find_first_of(',')));
        action = action.substr(action.find_first_of(',') + 1);
        destination.y = std::stoi(action.substr(0, action.find_first_of(',')));
        action = action.substr(action.find_first_of(',') + 1);
        
        //Erase the "move_creature_at_"
        action.erase(0, 17);
        
        //Parse the original tile from the string
        glm::ivec2 currentTile;
        currentTile.x = std::stoi(action.substr(0, action.find_first_of('_')));
        action = action.substr(action.find_first_of('_') + 1);
        currentTile.y = std::stoi(action);
        
        if (this->board.get(currentTile.x, currentTile.y).creature() != nullptr && this->board.destinationInRange(destination, currentTile) && this->board.get(currentTile.x, currentTile.y).creature()->controller() == playerNum) {
            std::vector<unsigned int> directions = this->players[playerNum].getPath(currentTile.x, currentTile.y, destination.x, destination.y);
            
            for (int a = 0; a < directions.size(); a++) {
                this->board.get(currentTile.x, currentTile.y).creature()->directions.push(directions[a]);
                if (a == 0)
                    this->board.setDirection(currentTile.x, currentTile.y, directions[a]);
            }
        }
    } else if (action.find("attack_from_") != std::string::npos) {
        //Parse the destination tile from the string
        glm::ivec2 destination;
        destination.x = std::stoi(action.substr(0, action.find_first_of(',')));
        action = action.substr(action.find_first_of(',') + 1);
        destination.y = std::stoi(action.substr(0, action.find_first_of(',')));
        action = action.substr(action.find_first_of(',') + 1);
        
        //Erase the "attack_from_"
        action.erase(0, 12);
        
        //Parse the original tile from the string
        glm::ivec2 currentTile;
        currentTile.x = std::stoi(action.substr(0, action.find_first_of('_')));
        action = action.substr(action.find_first_of('_') + 1);
        currentTile.y = std::stoi(action);
        
        if (this->board.get(currentTile.x, currentTile.y).creature() != nullptr && this->board.attackInRange(destination, currentTile) && this->board.get(currentTile.x, currentTile.y).creature()->controller() == playerNum && this->board.get(destination.x, destination.y).creature()->controller() != playerNum) {
            glm::ivec2 attacker = glm::ivec2(currentTile.x, currentTile.y);
            glm::ivec2 defender = glm::ivec2(destination.x, destination.y);
            
            if (this->board.tileDistances(attacker.x, attacker.y, defender.x, defender.y) <= this->board.get(attacker.x, attacker.y).creature()->range()) {
                
                int attackDamage = 0, defendDamage = 0;
                
                std::vector<std::string> actions = this->board.initiateCombat(attacker.x, attacker.y, defender.x, defender.y, &attackDamage, &defendDamage);
                this->board.setDamage(defender.x, defender.y, attackDamage, this->lastFrame.count()); //Make the damage visible
                this->board.setDamage(attacker.x, attacker.y, defendDamage, this->lastFrame.count()); //For attacker and defender
                
                for (int a = 0; a < actions.size(); a++) {
                     this->processAction(actions[a], -1); //playerNum doesn't matter
                }
            }
        }
    }
    
    
    
    
    
    
    //Process actions here
    if (action.find("make_creature,") != std::string::npos) { //Basically if the string action contains "make_creature", the button makes a creature
        
        glm::ivec2 selectedTile = this->players[playerNum].tileSelected();
        
        if (selectedTile != NO_SELECTION && selectedTile != INTERFACE_BOX_SELECTION && !this->board.get(selectedTile.x, selectedTile.y).occupied()) {

            //Interpret the string to find out what kind of creature

            /* The contents of the button string are:
             * creature,[race],[maxHealth],[maxEnergy],[attack],[vision],[range],[start direction]
             *
             * Each value in brackets indicates a number or enum that represents that value. Each of these values are separated by commas.
             *
             * This function goes through the string and extracts those values and constructs a creature based on them.
             */

            Race race = Human;
            AttackStyle attackStyle = Melee;
            int values[] = {0, 0, 0, 0, 0, 0};
            int direction;

            action.erase(0, 14); //Gets rid of the "make_creature," from the string

            //Extract the race of the creature

            if (action.compare(0, 5, "Human") == 0) {
                race = Human;
                action.erase(0, 6); //Gets rid of "Human,". This is 1 more than the number of characters in "Human" to also get rid of the comma. Same is true with the other races.
            } else if (action.compare(0, 3, "Elf") == 0) {
                race = Elf;
                action.erase(0, 4);
            } else if (action.compare(0, 5, "Dwarf") == 0) {
                race = Dwarf;
                action.erase(0, 6);
            } else if (action.compare(0, 3, "Orc") == 0) {
                race = Orc;
                action.erase(0, 4);
            } else if (action.compare(0, 6, "Goblin") == 0) {
                race = Goblin;
                action.erase(0, 7);
            } else if (action.compare(0, 6, "Undead") == 0) {
                race = Undead;
                action.erase(0, 7);
            } else if (action.compare(0, 7, "Vampire") == 0) {
                race = Vampire;
                action.erase(0, 8);
            } else {
                throw std::invalid_argument("Error adding creature: unreadable creature race");
            }

            //Extract the numerical values of the creature

            for (int valueNum = 0; valueNum < 5; valueNum++) {
                //Find the position of the next comma, which is the number of digits before that comma
                int numDigits = (int)action.find(',');

                for (int place = numDigits - 1; place >= 0; place--) {
                    values[valueNum] += ((int)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
                    action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
                }

                action.erase(0, 1); //Get rid of the comma
            }

            if (action.compare(0, 5, "NORTH") == 0) {
                direction = NORTH;
                action.erase(0, 6); //Gets rid of "NORTH,". This is 1 more than the number of characters in "NORTH" to also get rid of the comma. Same is true with the other directions.
            } else if (action.compare(0, 4, "EAST") == 0) {
                direction = EAST;
                action.erase(0, 5);
            } else if (action.compare(0, 5, "SOUTH") == 0) {
                direction = SOUTH;
                action.erase(0, 6);
            } else if (action.compare(0, 4, "WEST") == 0) {
                direction = WEST;
                action.erase(0, 5);
            } else {
                throw std::invalid_argument("Error adding creature: unreadable creature direction");
            }

            Creature newCreature(selectedTile.x, selectedTile.y, race, values[0], values[1], values[2], attackStyle, values[3], values[4], direction, playerNum);

            if (this->board.get(selectedTile.x, selectedTile.y).passableByCreature(newCreature)) {
                this->board.setCreature(selectedTile.x, selectedTile.y, newCreature);

                this->players[playerNum].resetAllTiles();
                
                this->players[playerNum].selectTile(NO_SELECTION.x, NO_SELECTION.y);
            }
        }
    } else if (action.find("building_new_creature") != std::string::npos) { //Basically if the string action contains "building", the button follows the building instructions
        
        //For now, set adjacent spots as reachable and create a creature on the selected one

        action.erase(0, 22); //Delete "building_new_creature(" from the action string

        glm::ivec2 buildingPos = glm::ivec2(0, 0);

        //Extract the building position

        int numDigits = (int)action.find(',');

        for (int place = numDigits - 1; place >= 0; place--) {
            buildingPos.x += ((int)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
            action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
        }

        action.erase(0, 1); //Get rid of the comma

        numDigits = (int)action.find(')');

        for (int place = numDigits - 1; place >= 0; place--) {
            buildingPos.y += ((int)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
            action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
        }

        action.erase(0, 1); //Get rid of the parenthasis

        //If the position is within the board
        if (buildingPos.x >= 0 && buildingPos.x < this->board.width() && buildingPos.y >= 0 && buildingPos.y < this->board.height(buildingPos.x)) {

            if (this->board.get(buildingPos.x, buildingPos.y).building() != nullptr && playerNum == this->board.get(buildingPos.x, buildingPos.y).building()->controller()) {

                //North
                if (buildingPos.y > 0) {
                    this->players[playerNum].setStyle(buildingPos.x, buildingPos.y - 1, REACHABLE);
                }

                //East
                if (buildingPos.x > 0) {
                    this->players[playerNum].setStyle(buildingPos.x - 1, buildingPos.y, REACHABLE);
                }

                //South
                if (buildingPos.y < this->board.height(buildingPos.x) - 1) {
                    this->players[playerNum].setStyle(buildingPos.x, buildingPos.y + 1, REACHABLE);
                }

                //West
                if (buildingPos.y < this->board.width() - 1) {
                    this->players[playerNum].setStyle(buildingPos.x + 1, buildingPos.y, REACHABLE);
                }
            }
        }
    } else if (action.find("make_building,") != std::string::npos) { //Basically if the string action contains "make_building", the button makes a building
        
        glm::ivec2 selectedTile = this->players[playerNum].tileSelected();
        
        if (selectedTile != NO_SELECTION && selectedTile != INTERFACE_BOX_SELECTION && !this->board.get(selectedTile.x, selectedTile.y).occupied()) {

            //Interpret the string to find out what kind of building

            /* The contents of the button string are:
             * building,[maxHealth]
             *
             * Each value in brackets indicates a number or enum that represents that value. Each of these values are separated by commas.
             *
             * This function goes through the string and extracts those values and constructs a building based on them.
             */
            int maxHealth;

            action.erase(0, 14); //Gets rid of the "make_building," from the string

            //Extract the numerical values of the building
            
            //Find the position of the next comma, which is the number of digits before that comma
            GLuint numDigits = (GLuint)action.find(',');
            
            for (int place = numDigits - 1; place >= 0; place--) {
                maxHealth += ((GLuint)action[0] - 48) * pow(10, place); //Converts the digit to an int and multiplies it by the right power of 10
                action.erase(0, 1); //Get the next digit, correctly add it to the value, and delete it from the string
            }

            action.erase(0, 1); //Get rid of the comma
            
            Building newBuilding(selectedTile.x, selectedTile.y, maxHealth, playerNum);
            
            if (!this->board.get(selectedTile.x, selectedTile.y).occupied()) {
                this->board.setBuilding(selectedTile.x, selectedTile.y, newBuilding);
                
                //Reset all tiles to be unselected now that the creature has been added
                this->players[playerNum].resetAllTiles();
                
                this->players[playerNum].selectTile(NO_SELECTION.x, NO_SELECTION.y);
            }
        }
    }
    
}

void Host::getBufferData(std::vector<int>* terrainData, std::vector<int>* creatureData, std::vector<std::vector<float> >* colorDataVec, std::vector<int>* damageData, std::vector<float>* offsetData, std::vector<int>* buildingData) {
    
    if (colorDataVec->size() < this->socket.numberOfClients())
        throw std::invalid_argument("Argument colorDataVec too small");
    
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            (*terrainData).push_back(this->board.get(x, y).terrain());
            
            (*creatureData).push_back(this->board.get(x, y).creatureType());
            if (this->board.get(x, y).creature() != nullptr) { //If there is a creature set the data properly, otherwise as 0
                (*creatureData).push_back(this->board.get(x, y).creature()->direction());
                (*creatureData).push_back(this->board.get(x, y).creature()->controller());
            } else {
                (*creatureData).push_back(0);
                (*creatureData).push_back(0);
            }
            
            for (int a = 0; a < this->socket.numberOfClients(); a++) {
                glm::vec3 tileColor = this->players[a].tileColor(x, y);
                (*colorDataVec)[a].push_back(tileColor.x);
                (*colorDataVec)[a].push_back(tileColor.y);
                (*colorDataVec)[a].push_back(tileColor.z);
            }
            
            (*damageData).push_back(this->board.get(x, y).damage());
            
            if (this->board.get(x, y).creature() != nullptr)
                (*offsetData).push_back(this->board.get(x, y).creature()->offset());
            else
                (*offsetData).push_back(0);
            
            (*buildingData).push_back(this->board.get(x, y).buildingType());
            if (this->board.get(x, y).building() != nullptr) //If there is a building set the data properly, otherwise as 0
                (*buildingData).push_back(this->board.get(x, y).building()->controller());
            else
                (*buildingData).push_back(0);
        }
    }
}
