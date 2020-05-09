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

struct Power : public Container{

    Container* base;
    Container* expo;
    void print();
    Container* copy();
    Container* eval();
    Container* tryToConst(Container* current);
    Container* convertToSinglePower(Container* current);
    bool equalStruct(Container* c);
    Power(Container* base,Container* expo);
    ~Power();
};

#endif /* Power_hpp */