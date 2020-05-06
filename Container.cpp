//
//  Container.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Container.hpp"

int Container::numberOfContainers = 0;
void Container::printNumberOfContainers(){
    printf("\n%d\n",numberOfContainers);
}

Container::Container(){
    numberOfContainers++;
};
Container::~Container(){
    numberOfContainers--;
};
