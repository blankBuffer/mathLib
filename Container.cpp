//
//  Container.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Container.hpp"
#include "ConTypes.hpp"

int Container::conCount = 0;
bool Container::printSteps = false;

Container::Container(){
    conCount++;
};
Container::~Container(){
    conCount--;
};
