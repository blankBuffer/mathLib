//
//  Var.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Var_hpp
#define Var_hpp

#include <stdio.h>
#include "Container.hpp"

struct Var: public Container{
    char* name;
    int nameLength;
    Var(char* name,int nameLength);
    void print();
    Container* copy();
    Container* eval();
    bool equalStruct(Container* c);
    bool containsVars();
    bool containsContainer(Container* c);
    ~Var();
};

#endif /* Var_hpp */
