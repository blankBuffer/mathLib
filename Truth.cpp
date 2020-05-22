//
//  Truth.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/14/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Truth.hpp"
#include "Const.hpp"

Truth::Truth(Container* left,Container* right){
    this->left = left;
    this->right = right;
}

void Truth::print(){
    left->print();
    printf("=");
    right->print();
}

void Truth::bothSidesTheSame(Truth* current){
    
    if(current->left->equalStruct(current->right)){
        if(Container::printSteps) printf("\nboth sides the same\n");
        delete current->left;
        delete current->right;
        current->left = new Const(0);
        current->right = new Const(0);
    }
    
}

Truth* Truth::eval(){
    Container* currentLeft = left->eval();
    Container* currentRight = right->eval();
    Truth* current = new Truth(currentLeft,currentRight);
    
    bothSidesTheSame(current);
    
    return current;
    
}
Truth::~Truth(){
    delete left;
    delete right;
}
