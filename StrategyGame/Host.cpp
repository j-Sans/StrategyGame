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
        this->alivePlayers.push_back({true, true});
    }
    
    //Send to each player that player's number and the board
    for (int a = 0; a < numberOfPlayers; a++) {
        this->socket.send(std::to_string(a) + "," + this->board.serialize(), a);
    }
    if (!this->receivedFromAll("initialDataReceived"))
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
    
    //Upkeep (Energy is incremented by 1 every 5 seconds);
//    if (currentFrame.count() - timeOfLastUpkeep > timeBetweenUpkeep) {
//        this->board.upkeep();
//        timeOfLastUpkeep = currentFrame.count();
//        std::cout << "\n\nupkeep counter reset\n\n";
//    }
    
    for (int a = 0; a < this->players.size(); a++) {
        this->players[a].updateCreatures(this->deltaTime);
    }
    
    this->broadcast(this->board.serialize());

    for (int a = 0; a < this->socket.numberOfClients(); a++) {
        std::string clientInfo = this->socket.receive(a);
        while (clientInfo.size() > 0) {
            this->processAction(clientInfo.substr(0, clientInfo.find_first_of(';')), a); //Process the action
            clientInfo = clientInfo.find_first_of(';') == std::string::npos ? "" : clientInfo.substr(clientInfo.find_first_of(';') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
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
    if (!this->alivePlayers[playerNum].first) return; //If the player is dead, ignore any remaining actions.
    
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
                
                std::vector<std::pair<std::string, int> > actions = this->board.initiateCombat(attacker.x, attacker.y, defender.x, defender.y, &attackDamage, &defendDamage);
                this->board.setDamage(defender.x, defender.y, attackDamage, this->lastFrame.count()); //Make the damage visible
                this->board.setDamage(attacker.x, attacker.y, defendDamage, this->lastFrame.count()); //For attacker and defender
                
                for (int a = 0; a < actions.size(); a++) {
                    this->processAction(actions[a].first, actions[a].second); //playerNum doesn't matter
                }
            }
        }
    } else if (action.find("player_lose_") != std::string::npos) {
        action.erase(0, 12); //Erases "player_lose_"
        int playerToLose = std::stoi(action);
        this->losePlayer(playerToLose);
    } else if (action.find("leaving_game_player_") != std::string::npos) {
        action.erase(0, 20); //Erases "leaving_game_player_"
        int leavingPlayer = std::stoi(action);
        this->losePlayer(leavingPlayer);
        this->alivePlayers[leavingPlayer].second = false; //Set the player to disconnected.
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

void Host::losePlayer(int playerNum) {
    if (playerNum < 0 || playerNum >= this->players.size()) {
        throw std::range_error("playerNum (" + std::to_string(playerNum) + ") not a valid player. Max player index: " + std::to_string(this->players.size()));
    } else if (this->alivePlayers[playerNum].first == false) {
        throw std::logic_error("Player " + std::to_string(playerNum) + " already dead");
    }
    
    this->alivePlayers[playerNum].first = false;
    
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            Creature* creature = this->board.get(x, y).creature();
            if (creature != nullptr) {
                this->processAction(this->board.deleteCreature(x, y), creature->controller());
            }
            Building* building = this->board.get(x, y).building();
            if (building != nullptr) {
                this->processAction(this->board.deleteBuilding(x, y), building->controller());
            }
        }
    }
}

void Host::broadcast(std::string message) {
    for (int player = 0; player < this->players.size(); player++) {
        if (!this->alivePlayers[player].second) {
            continue; //Skip if the player disconnected;
        } else if (!this->alivePlayers[player].first) { //Test if the dead player is still connected
            try {
                this->socket.send(message, player);
            } catch (std::runtime_error) {
                this->alivePlayers[player].second = false; //If the player didn't receive it, set that player to disconnected.
                std::cout << "Player " << player << " (dead) didn't receive " << message << std::endl;
            }
        } else {
            try {
                this->socket.send(message, player);
            } catch (std::runtime_error) {
                this->losePlayer(player); //If the player didn't receive it, that player disconnected. Remove that player.
                this->alivePlayers[player].second = false;
                std::cout << "Player " << player << " (alive) didn't receive " << message << std::endl;
            }
        }
    }
}

bool Host::receivedFromAll(std::string str) {
    for (int player = 0; player < this->players.size(); player++) {
        if (!this->alivePlayers[player].first) continue; //Skip if the player is dead
        if (this->socket.receive(player) != str) return false;
    }
    return true;
}
