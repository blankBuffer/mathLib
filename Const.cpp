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
    isContainer = false;
    type = CONST;
}
Const::Const(Container* container){
    this->container = container;
    isContainer = true;
    type = CONST;
}
void Const::print(){
    if(!isContainer) printf("%ld",value);
    else container->print();
}
Container* Const::copy(){
    if(!isContainer)return new Const(value);
    else return container->copy();
}
Container* Const::eval(){
    if(!isContainer)return new Const(value);
    else return container->eval();
}
bool Const::equalStruct(Container* c){
    if(c->type == CONST){
        Const* other = (Const*)c;
        if(!isContainer && !other->isContainer){
            if(value==other->value) return true;
        }else if(isContainer && other->isContainer){
            return container->equalStruct(other->container);
        }
    }
    return false;
}
Const::~Const(){
    if(isContainer) delete container;
};
