//
//  Container.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Container.hpp"

int Container::conCount = 0;

Container::Container(){
    conCount++;
};
Container::~Container(){
    conCount--;
};
