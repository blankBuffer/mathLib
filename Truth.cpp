//
//  Truth.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/14/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Truth.hpp"

Truth::Truth(Container* left,Container* right){
    this->left = left;
    this->right = right;
}

Truth::~Truth(){
    delete left;
    delete right;
}
