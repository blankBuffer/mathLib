//
//  Var.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Var.hpp"

Var::Var(char* name,int nameLength){
    this->name = name;
    this->nameLength = nameLength;
    type = VAR;
}
void Var::print(){
    printf("%s",name);
}
Container* Var::copy(){
    return this;
}
Container* Var::eval(){
    return this;
}
bool Var::equalStruct(Container* c){
    if(c==this) return true;
    return false;
}
Var::~Var(){};
