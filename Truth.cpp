//
//  Truth.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/14/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Truth.hpp"
#include "Const.hpp"
#include "Product.hpp"
#include "Sum.hpp"

Truth::Truth(Container* left,Container* right){
    this->left = left;
    this->right = right;
}

void Truth::print(){
    left->print();
    printf("=");
    right->print();
}

Truth* Truth::eval(){
    
    Container** sumList = new Container*[2];
    sumList[0] = right->copy();
    
    Container** prodList = new Container*[2];
    prodList[0] = left->copy();
    prodList[1] = new Const(-1);
    
    sumList[1] = new Product(prodList,2);
    
    Sum* allOneSide = new Sum(sumList,2);
    Container* allOneSideEval = allOneSide->eval();
    delete allOneSide;
    return new Truth( new Const(0), allOneSideEval );
}

Truth::~Truth(){
    delete left;
    delete right;
}
