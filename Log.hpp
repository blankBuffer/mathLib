//
//  Log.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Log_hpp
#define Log_hpp

#include <stdio.h>
#include "Container.hpp"

struct Log: public Container{
    Container* base, *expr;
    Log(Container* base,Container* expr);
    
    void print();
    Container* copy();
    Container* eval();
    bool equalStruct(Container* c);
    bool containsVars();
    bool containsContainer(Container* c);
    Container* baseExprSame(Container* current);
    Container* containsPower(Container* current);
    Container* containsProduct(Container* current);
    ~Log();
};

#endif /* Case_hpp */
