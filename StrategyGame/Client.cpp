//
//  Client.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 10/1/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Client.hpp"

Client::Client(Window* w, ClientSocket *socket, bool* mouseDown, bool* mouseUp, bool* returnToMenu, bool* keys) : visualizer(Visualizer(w, "Shaders/board/board.vert", "Shaders/board/board.geom", "Shaders/board/board.frag", mouseDown, mouseUp, keys)), board(Board(std::vector<std::vector<Tile> >(0))) {
    
    this->returnToMenu = returnToMenu;
    
    this->socket = socket;
    
    std::string initialInfo = this->socket->receive();
    if (initialInfo == "starting") {
        this->socket->send("message_received");
        initialInfo = this->socket->receive();
    }
    this->socket->send("initialDataReceived");
    
    this->playerNum = std::stoi(initialInfo.substr(0, initialInfo.find_first_of(',')));
    initialInfo = initialInfo.substr(initialInfo.find_first_of(',') + 1, std::string::npos); //Set the string equal to the rest of the string after the ','
    
    this->board = Board::deserialize(initialInfo);
    
    //Initialize the various 2D vectors that hold information for each tile
    for (int x = 0; x < this->board.width(); x++) {
        std::vector<std::array<int, 2> > boardInfoColumn(this->board.height(x));
        std::vector<std::queue<std::string> > tileActionColumn(this->board.height(x));
        this->boardInfo.push_back(boardInfoColumn);
        this->tileActions.push_back(tileActionColumn);
    }
    
    //Give the buffer data to the visualizer
    this->getBufferData(&this->visualizer.terrainData, &this->visualizer.creatureData, &this->visualizer.colorData, &this->visualizer.damageData, &this->visualizer.offsetData, &this->visualizer.buildingData);
    
    
    this->setInterfaces();
    this->updateInterfaces();
    
    //Set the visualizer
    this->visualizer.set(this->board.width(), this->board.height(0));
}

void Client::render() {
//    this->visualizer.startFrame();
//
    
    std::string str = this->socket->receive();
    if (str == "closing_host") {
        *this->returnToMenu = true;
        return;
    }
    this->board = Board::deserialize(str);
    
    //If the window will be closing notify the Host
    if (this->visualizer.window->shouldClose()) this->actionsForClientInfo.push_back("leaving_game_player_" + std::to_string(this->playerNum));
    
    std::string clientInfo = "";
    
    for (auto a = this->visualizer.actions.begin(); a != this->visualizer.actions.end(); a++) {
        this->processAction(*a);
        a = this->visualizer.actions.erase(a);
    }
    
    for (auto a = this->actionsForClientInfo.begin(); a != this->actionsForClientInfo.end(); a++) {
        clientInfo += *a + ";";
        a = this->actionsForClientInfo.erase(a);
    }
    
    if (clientInfo == "") {
        clientInfo = "no_updates";
    }
    
    this->socket->send(clientInfo.c_str());
    
    this->updateSelected(this->visualizer.mousePressed(), this->visualizer.getTile(), glfwGetTime());
    
    this->updateInterfaces();
    
    this->getBufferData(&this->visualizer.terrainData, &this->visualizer.creatureData, &this->visualizer.colorData, &this->visualizer.damageData, &this->visualizer.offsetData, &this->visualizer.buildingData);
    
    this->visualizer.render();
//    
//    this->visualizer.endFrame();
}

bool Client::getShouldWindowClose() {
    return this->visualizer.window->shouldClose();
}

void Client::resetAllTiles() {
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            //Set all tile styles to regular
            this->boardInfo[x][y][TILE_STYLE] = REGULAR;
            
            //Empty all queues of action for each tile
            while (this->tileActions[x][y].size() > 0)
                this->tileActions[x][y].pop();
        }
    }
}

void Client::updateSelected(bool mouseDown, glm::ivec2 mousePos, unsigned int currentTime) {
    //Update all tiles other than the one where the mouse is to have no hovering
    for (int x = 0; x < this->board.width(); x++) {
        for (int y = 0; y < this->board.height(x); y++) {
            this->boardInfo[x][y][TILE_HOVER] = NO_HOVERING;
        }
    }
    
    if (this->board.validTile(mousePos)) {
        this->boardInfo[mousePos.x][mousePos.y][TILE_HOVER] = HOVERING;
    }
    
    if (mouseDown) {
        if (mousePos == NO_SELECTION) {
            this->resetAllTiles();
        } else if (mousePos == INTERFACE_BOX_SELECTION) {
        } else if (mousePos == this->selectedTile) { //Reset the tile (and others) if the current tile is clicked again
            this->resetAllTiles();
            this->selectedTile = NO_SELECTION;
        } else {
            switch (this->boardInfo[mousePos.x][mousePos.y][TILE_STYLE]) {
                    
                case REGULAR: { //If it is an empty spot, change the selected tile to that spot and reset the old selected tile
                    this->resetAllTiles();
                    this->selectedTile = mousePos;
                    
                    //Select this new tile
                    this->boardInfo[mousePos.x][mousePos.y][TILE_STYLE] = SELECTED;
                    
                    Creature* creature = this->board.get(mousePos.x, mousePos.y).creature();
                    
                    //If the selected tile is a creature, highlight reachable tiles and update the creature's direction
                    if (creature != nullptr && creature->controller() == this->playerNum) {
                        std::vector<Tile> reachableTiles = this->board.getReachableTiles(this->board.get(mousePos.x, mousePos.y));
                        
                        for (int a = 0; a < reachableTiles.size(); a++) {
                            if (this->board.get(reachableTiles[a].x(), reachableTiles[a].y()).passableByCreature(*creature)) {
                                this->boardInfo[reachableTiles[a].x()][reachableTiles[a].y()][TILE_STYLE] = REACHABLE;
                                this->tileActions[reachableTiles[a].x()][reachableTiles[a].y()].push("move_creature_at_" + std::to_string(this->selectedTile.x) + "_" + std::to_string(this->selectedTile.y));
                            }
                        }
                        
                        if (creature->energy() > 0) {
                            std::vector<Tile> attackableTiles = this->board.getAttackableTiles(this->board.get(mousePos.x, mousePos.y));
                            
                            for (int a = 0; a < attackableTiles.size(); a++) {
                                //If there is a creature or building on the tile, controlled by an opponent, make it attackable
                                if ((this->board.get(attackableTiles[a].x(), attackableTiles[a].y()).creature() != nullptr && this->board.get(attackableTiles[a].x(), attackableTiles[a].y()).creature()->controller() != this->playerNum) || (this->board.get(attackableTiles[a].x(), attackableTiles[a].y()).building() != nullptr && this->board.get(attackableTiles[a].x(), attackableTiles[a].y()).building()->controller() != this->playerNum)) {
                                    this->boardInfo[attackableTiles[a].x()][attackableTiles[a].y()][TILE_STYLE] = ATTACKABLE;
                                    this->tileActions[attackableTiles[a].x()][attackableTiles[a].y()].push("attack_from_" + std::to_string(this->selectedTile.x) + "_" + std::to_string(this->selectedTile.y));
                                }
                            }
                        }
                    }
                    break;
                } case REACHABLE: {
                    this->resolveTileAction(mousePos.x, mousePos.y);
                    this->resetAllTiles();
                    this->selectedTile = NO_SELECTION;
                    break;
                } case ATTACKABLE: {
                    this->resolveTileAction(mousePos.x, mousePos.y);
                    this->resetAllTiles();
                    this->selectedTile = NO_SELECTION;
                }
            }
        }
    }
}

void Client::setInterfaces() {
    this->interfaces = this->visualizer.getInterfaces();
}

void Client::updateInterfaces() {
    this->visualizer.leftInterface = &this->interfaces[default_left];
    this->visualizer.bottomInterface = &this->interfaces[default_bottom];
    this->visualizer.rightInterface = &this->interfaces[default_right];
    
    //If the selected tile is on the board
    if (this->board.validTile(this->selectedTile)) {
        Tile tile = this->board.get(this->selectedTile.x, this->selectedTile.y);
        
        if (tile.creature() != nullptr) {
            //Set the right interface to be the creature if there is a creature at the selected tile
            this->visualizer.rightInterface = &this->interfaces[creature];
            
            //Update the boxMap to display creature stats
            if (this->interfaces[creature].boxMap.size() > 0) {
                this->interfaces[creature].boxMap[creature_attack].text = "Attack: " + std::to_string(tile.creature()->attack());
                this->interfaces[creature].boxMap[creature_range].text = "Range: " + std::to_string(tile.creature()->range());
                this->interfaces[creature].boxMap[creature_vision].text = "Vision: " + std::to_string(tile.creature()->vision());
                this->interfaces[creature].boxMap[creature_race].text = tile.creature()->raceString();
            }
            
            //Update the display bars to display the creature quantities, like health and energy, which change
            if (this->interfaces[creature].displayBars.size() > 0) {
                
                this->interfaces[creature].displayBars[HealthBar].setValue(tile.creature()->health());
                this->interfaces[creature].displayBars[HealthBar].setMaxValue(tile.creature()->maxHealth());
                this->interfaces[creature].displayBars[HealthBar].text = "Health: " + std::to_string((int)tile.creature()->health()) + "/" + std::to_string((int)tile.creature()->maxHealth());
                
                this->interfaces[creature].displayBars[EnergyBar].setValue(tile.creature()->energy());
                this->interfaces[creature].displayBars[EnergyBar].setMaxValue(tile.creature()->maxEnergy());
                this->interfaces[creature].displayBars[EnergyBar].text = "Energy: " + std::to_string((int)tile.creature()->energy()) + "/" + std::to_string((int)tile.creature()->maxEnergy());
            }
        } else if (tile.building() != nullptr) {
            //Do the same for buildings
            this->visualizer.rightInterface = &this->interfaces[building];
            
            if (this->interfaces[building].displayBars.size() > 0) {
                this->interfaces[building].displayBars[HealthBar].setValue(tile.building()->health());
                this->interfaces[building].displayBars[HealthBar].setMaxValue(tile.building()->maxHealth());
                this->interfaces[building].displayBars[HealthBar].text = "Health: " + std::to_string((int)tile.building()->health()) + "/" + std::to_string((int)tile.building()->maxHealth());
            }
            
            if (this->interfaces[building].buttons.size() > 0) {
//                this->interfaces[building].buttons[0].text = tile.building()->buttonText();
//                this->interfaces[building].buttons[0].action = tile.building()->action();
            }
        }
    }
}

void Client::getBufferData(std::vector<int>* terrainData, std::vector<int>* creatureData, std::vector<float>* colorData, std::vector<int>* damageData, std::vector<float>* offsetData, std::vector<int>* buildingData) {
    
    //Empty the vectors before pushing them back
    (*terrainData).clear();
    (*creatureData).clear();
    (*colorData).clear();
    (*damageData).clear();
    (*offsetData).clear();
    (*buildingData).clear();
    
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
            
            glm::vec3 tileColor = this->tileColor(x, y);
            (*colorData).push_back(tileColor.x);
            (*colorData).push_back(tileColor.y);
            (*colorData).push_back(tileColor.z);
            
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

glm::vec3 Client::tileColor(unsigned int x, unsigned int y) {
    if (x >= this->board.width())
        throw std::range_error("X out of range");
    if (y >= this->board.height(x))
        throw std::range_error("Y out of range");
    
    int style = this->boardInfo[x][y][TILE_STYLE];
    int hover = this->boardInfo[x][y][TILE_HOVER];
    
    if (style == REGULAR)
        return hover == NO_HOVERING ? WHITE : WHITE * HOVER_EFFECT;
    else if (style == SELECTED)
        return hover == NO_HOVERING ? GREY : GREY * HOVER_EFFECT;
    else if (style == ATTACKABLE)
        return hover == NO_HOVERING ? RED : RED * HOVER_EFFECT;
    else if (style == REACHABLE)
        return hover == NO_HOVERING ? GREEN : GREEN * HOVER_EFFECT;
    
    //Something went wrong. Return White to have an unaltered color
    return WHITE;
}

void Client::processAction(std::string action) {
    if (action == "return_to_menu") {
        *this->returnToMenu = true;
        this->actionsForClientInfo.push_back("leaving_game_player_" + std::to_string(this->playerNum));
    } else {
        this->actionsForClientInfo.push_back(action);
    }
}

void Client::resolveTileAction(int x, int y) {
    if (x > this->tileActions.size())
        throw std::range_error("X out of bounds");
    if (this->tileActions.size() < 1)
        throw std::logic_error("No board size");
    if (y > this->tileActions[x].size())
        throw std::range_error("Y out of bounds");
    if (this->tileActions[x][y].size() == 0)
        throw std::logic_error("No actions to call");
    
    std::string action = this->tileActions[x][y].front();
    this->tileActions[x][y].pop();
    
    this->actionsForClientInfo.push_back(std::to_string(x) + "," + std::to_string(y) + "," + action);
}
