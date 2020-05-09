//
//  Const.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Const.hpp"
#include "Container.hpp"

Const::Const(long value){
    this->value = value;
    type = CONST;
}
void Const::print(){
    printf("%ld",value);
}
Container* Const::copy(){
    return new Const(value);
}
Container* Const::eval(){
    return copy();
}
bool Const::equalStruct(Container* c){
    if(c->type == CONST){
        Const* other = (Const*)c;
        if(value==other->value) return true;
    }
    return false;
}
Const::~Const(){
};
