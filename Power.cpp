//
//  Power.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/7/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Power.hpp"
#include "Const.hpp"
#include "Product.hpp"

void Power::print(){
    printf("((");
    base->print();
    printf(")^");
    expo->print();
    printf(")");
}
Container* Power::copy(){
    return new Power(base->copy(),expo->copy());
}
Container* Power::tryToConst(Container* current){
    if(current->type!=POW) return current;
    Power* currentPow = (Power*)current;
    if(currentPow->base->type != CONST) return current;
    if(currentPow->expo->type != CONST) return current;
    long cBase = ((Const*)currentPow->base)->value;
    long cExpo = ((Const*)currentPow->expo)->value;
    if (cExpo == -1) return current;
    //start
    if(Container::printSteps) printf("\nturning power of constants to simple form\n");
    delete current;
    if(cExpo>0){
        long product = 1L;
        for(int i = 0;i<cExpo;i++) product*=cBase;
        return new Const(product);
    }else if(cExpo<0){
        long product = 1L;
        for(int i = 0;i<-cExpo;i++) product*=cBase;
        Container* rBase = new Const(product);
        return new Power(rBase,new Const(-1L));
    }else{
        return new Const(1);
    }
}
Container* Power::convertToSinglePower(Container* current){
    if(current->type!=POW) return current;
    Power* currentPow = (Power*)current;
    if(currentPow->base->type != POW) return current;
    //make one power container
    if(Container::printSteps) printf("\nturning into one exponent\n");
    Power* basePower = (Power*)currentPow->base;
    Container** list = new Container*[2];
    list[0] = expo->copy();
    list[1] = basePower->expo->copy();
    Product* old = new Product(list,2);
    Container* expo = old->eval();
    delete old;
    Container* base = basePower->base->copy();
    delete current;
    return new Power(base,expo);
}
Container* Power::eval(){
    Container* current = this->copy();
    current = convertToSinglePower(current);
    current = tryToConst(current);
    return current;
}
bool Power::equalStruct(Container* c){
    if(c->type != POW) return false;
    Power* other = (Power*)c;
    if(other->expo->equalStruct(this->expo) && other->base->equalStruct(this->base)) return true;
    return false;
}
Power::Power(Container* base,Container* expo){
    this->base = base;
    this->expo = expo;
    type = POW;
}
Power::~Power(){
    delete base;
    delete expo;
}
