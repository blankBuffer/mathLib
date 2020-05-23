//
//  Log.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Log.hpp"
#include "Const.hpp"
#include "Power.hpp"
#include "Product.hpp"
#include "Sum.hpp"

Log::Log(Container* base,Container* expr){
    this->base = base;
    this->expr = expr;
    this->type = LOG;
}

void Log::print(){
    printf("Log(");
    base->print();
    printf(",");
    expr->print();
    printf(")");
}
Container* Log::copy(){
    return new Log(base->copy(),expr->copy());
}

Container* Log::baseExprSame(Container* current){
    if(current->type != LOG) return current;
    Log* currentLog = (Log*)current;
    
    if(currentLog->base->equalStruct(currentLog->expr)){
        if(Container::printSteps) printf("\nbase and expression the same\n");
        delete currentLog;
        return new Const(1);
    }
    
    return current;
}

Container* Log::containsPower(Container* current){
    if(current->type != LOG) return current;
    Log* currentLog = (Log*)current;
    
    if(currentLog->expr->type == POW){
        if(Container::printSteps) printf("\nlog expression contains Power\n");
        Power* pwr = (Power*)(currentLog->expr);
        Container** prodList = new Container*[2];
        prodList[0] = pwr->expo->copy();
        prodList[1] = new Log(currentLog->base->copy(),pwr->base->copy());
        delete current;
        return new Product(prodList,2);
    }
    
    if(currentLog->base->type == POW){
        if(Container::printSteps) printf("\nlog base contains Power\n");
        Power* pwr = (Power*)(currentLog->base);
        Container** prodList = new Container*[2];
        prodList[0] = new Power(pwr->expo->copy(),new Const(-1));
        prodList[1] = new Log(pwr->base->copy(),currentLog->expr->copy());
        delete current;
        return new Product(prodList,2);
    }
    
    return current;
    
}

Container* Log::containsProduct(Container* current){
    if(current->type != LOG) return current;
    Log* currentLog = (Log*)current;
    
    if(currentLog->expr->type == PROD){
        Product* containedProd = (Product*)currentLog->expr;
        
        Container** sumList = new Container*[containedProd->containersLength];
        for(int i = 0;i<containedProd->containersLength;i++){
            sumList[i] = new Log(currentLog->base->copy(),containedProd->containers[i]->copy());
        }
        Sum* sum = new Sum(sumList,containedProd->containersLength);
        delete current;
        
        return sum;
    }
    
    return current;
}

Container* Log::eval(){
    if(Container::printSteps){
        printf("\n");
        printf("\nevaluating log\n");
        printf("\n");
    }
    if(Container::printSteps) this->print();
    Container* current = new Log(base->eval(),expr->eval());
    
    current = baseExprSame(current);//base same as expression log(x,x) -> 1
    current = containsProduct(current);//log(a,b*c) -> log(a,b)+log(a,c)
    current = containsPower(current);//contains exponent log(a,b^c) -> c*log(a,b) or log(a^b,c)->b^(-1)*log(a,c)
    
    
    return current;
}
bool Log::equalStruct(Container* c){
    if(c->type == LOG){
        Log* other = (Log*)c;
        bool baseTheSame = this->base->equalStruct(other->base);
        if(!baseTheSame) return false;
        bool exprTheSame = this->expr->equalStruct(other->expr);
        if(!exprTheSame) return false;
        return true;
    }
    return false;
}
bool Log::containsVars(){
    return base->containsVars()||expr->containsVars();
}
bool Log::containsContainer(Container* c){
    bool contains = false;
    contains = c->equalStruct(this);
    if(contains) return true;
    contains = this->base->containsContainer(c);
    if(contains) return true;
    contains = this->expr->containsContainer(c);
    if(contains) return true;
    return false;
}
    
Log::~Log(){
    delete base;
    delete expr;
}
