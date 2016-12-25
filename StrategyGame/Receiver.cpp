//
//  Receiver.cpp
//  Strategy Game
//
//  Created by Jake Sanders on 12/25/16.
//  Copyright © 2016 Jake Sanders. All rights reserved.
//

#include "Receiver.hpp"

void Receiver::render() {
    
}

void Receiver::terminate() {
    this->client.terminate();
}

bool Receiver::windowShouldClose() {
    return this->client.windowShouldClose();
}
