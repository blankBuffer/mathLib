//
//  Power.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/7/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Power_hpp
#define Power_hpp

#include <stdio.h>
#include "Container.hpp"
#include "Var.hpp"

struct Power : public Container{

    Container* base;
    Container* expo;
    void print();
    Container* copy();
    Container* eval();
    Container* tryToConst(Container* current);
    Container* convertToSinglePower(Container* current);
    Container* distributePower(Container* current);
    Container* exponentZero(Container* current);
    Container* baseZeroOrOne(Container* current);
    Container* fracExpo(Container* current);
    Container* invExpo(Container* current);
    Container* expoOne(Container* current);
    bool equalStruct(Container* c);
    bool containsVars();
    bool containsContainer(Container* c);
    int countVars(Var* v);
    Power(Container* base,Container* expo);
    ~Power();
};

#endif /* Power_hpp */
