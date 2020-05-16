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
    
    //jokes
    
    if(Container::printSteps){
        if(value == 42) printf("\nanswer to life the universe and everything\n");
        if(value == 666) printf("\noh no this software has hexakosioihexekontahexaphobia\n");
        if(value == 5040) printf("\ndivisible indeed\n");
        if(value == 4) printf("\nthis dimension has the most polytopes\n");
    }
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
bool Const::containsVars(){
    return false;
}
bool Const::containsContainer(Container* c){
    return c->equalStruct(this);
}
Const::~Const(){
};
