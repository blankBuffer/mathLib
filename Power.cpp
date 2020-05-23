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
    printf("(");
    base->print();
    printf(")^(");
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
Container* Power::distributePower(Container* current){
    if(current->type!=POW) return current;
    Power* currentPow = (Power*)current;
    if(currentPow->base->type != PROD) return current;
    if(Container::printSteps) printf("\ndistribute exponent\n");
    Product* prod = (Product*)currentPow->base;
    Container** list = new Container*[prod->containersLength];
    for(int i = 0;i<prod->containersLength;i++){
        Container* bs = prod->containers[i]->copy();
        Container* xp = currentPow->expo->copy();
        Power* sub = new Power(bs,xp);
        list[i] = sub;
    }
    Product* retOld = new Product(list,prod->containersLength);
    Container* ret = retOld->eval();
    delete retOld;
    delete current;
    return ret;
}
Container* Power::exponentZero(Container* current){
    if(current->type!=POW) return current;
    Power* currentPow = (Power*)current;
    if(currentPow->expo->type != CONST) return current;
    Const* c = (Const*)currentPow->expo;
    if(c->value != 0) return current;
    if(Container::printSteps) printf("\nexponent zero returning 1\n");
    delete current;
    return new Const(1);
}
Container* Power::baseZeroOrOne(Container* current){
    if(current->type!=POW) return current;
    Power* currentPow = (Power*)current;
    if(currentPow->base->type != CONST) return current;
    Const* b = (Const*)currentPow->base;
    if(b->value == 0){
        if(Container::printSteps) printf("\nbase zero returning 0\n");
        delete current;
        return new Const(0);
    }else if(b->value == 1){
        if(Container::printSteps) printf("\nbase 1 returning 1\n");
        delete current;
        return new Const(1);
    }
    return current;
}
Container* Power::fracExpo(Container* current){
    if(current->type!=POW) return current;
    Power* currentPow = (Power*)current;
    if(currentPow->base->type != CONST) return current;
    if(currentPow->expo->type != PROD) return current;
    Product* prod = (Product*)currentPow->expo;
    if(prod->containersLength!=2) return current;
    long num = 0;
    bool found = false;
    for(int i = 0;i<2;i++){
        if(prod->containers[i]->type == CONST){
            found = true;
            num = ((Const*)prod->containers[i])->value;
        }
    }
    if(!found) return current;
    found = false;
    long den = 0;
    for(int i = 0;i<2;i++){
        if(prod->containers[i]->type == POW){
            Power* p = (Power*)prod->containers[i];
            if(p->base->type == CONST && p->expo->type == CONST){
                Const* e = (Const*)p->expo;
                if(e->value == -1){
                    found = true;
                    den = ((Const*)p->base)->value;
                }
            }
        }
    }
    if(!found) return current;
    if(Container::printSteps) printf("\nsimplifiying fractional exponent\n");
    long base = ((Const*)currentPow->base)->value;
    long count = 1L;
    long res = 1L;
    while (res<base) {
        count++;
        res = 1;
        for(int i = 0;i<den;i++) res*=count;
    }
    if(res == base){
        delete current;
        res = 1L;
        for(int i = 0;i<num;i++) res*=count;
        return new Const(res);
    }
    return current;
    
}
Container* Power::invExpo(Container* current){
    if(current->type!=POW) return current;
    Power* currentPow = (Power*)current;
    if(currentPow->base->type != CONST) return current;
    if(currentPow->expo->type != POW) return current;
    Power* ex = (Power*)currentPow->expo;
    if(ex->base->type != CONST) return current;
    if(ex->expo->type != CONST) return current;
    if(((Const*)(ex->expo))->value != -1) return current;
    
    if(Container::printSteps) printf("\nsimplifiying inverse exponent\n");
    
    long base = ((Const*)currentPow->base)->value;
    long den = ((Const*)(ex->base))->value;
    
    long count = 1L;
    long res = 1L;
    while (res<base) {
        count++;
        res = 1;
        for(int i = 0;i<den;i++) res*=count;
    }
    if(res == base){
        return new Const(count);
    }
    return current;
    
}

Container* Power::expoOne(Container* current){
    if(current->type!=POW) return current;
    Power* currentPow = (Power*)current;
    
    if(currentPow->expo->type==CONST){
        Const* c = (Const*)currentPow->expo;
        if(c->value == 1){
            Container* b = currentPow->base->copy();
            delete current;
            return b;
        }
    }
    return current;
}

Container* Power::eval(){
    if(Container::printSteps){
        printf("\n");
        this->print();
        printf("\n");
    }
    if(Container::printSteps) printf("\nevaluating power\n");
    Container* current = new Power(base->eval(),expo->eval());
    current = convertToSinglePower(current);//(a^b)^c -> a^(b*c)
    current = tryToConst(current);//2^3 -> 8
    current = fracExpo(current);//8^(2/3) -> 4
    current = invExpo(current);//4^(1/2)->2
    current = exponentZero(current);//x^0->1
    current = baseZeroOrOne(current);//1^x->1 and 0^x->0
    current = expoOne(current);//x^1->x
    current = distributePower(current);//(a*b)^y->(a^y)*(b^y)
    return current;
}
bool Power::equalStruct(Container* c){
    if(c->type != POW) return false;
    Power* other = (Power*)c;
    if(other->expo->equalStruct(this->expo) && other->base->equalStruct(this->base)) return true;
    return false;
}
bool Power::containsVars(){
    return base->containsVars() || expo->containsVars();
}
bool Power::containsContainer(Container* c){
    bool contains = false;
    contains = c->equalStruct(this);
    if(contains) return true;
    contains = base->containsContainer(c);
    if(contains) return true;
    contains = expo->containsContainer(c);
    if(contains) return true;
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
