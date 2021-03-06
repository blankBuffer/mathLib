//
//  Container.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Container_hpp
#define Container_hpp

#include <stdio.h>

struct Var;

struct Container{
    static const int CONST = 0,VAR = 1,PROD = 2,POW = 3,SUM = 4,LOG = 5;
    static int conCount;
    static bool printSteps;
    int type;
    virtual void print() = 0;
    virtual Container* copy() = 0;
    virtual Container* eval() = 0;
    virtual bool equalStruct(Container* c) = 0;
    virtual bool containsVars() = 0;
    virtual bool containsContainer(Container* c) = 0;
    virtual int countVars(Var* v) = 0;
    Container();
    virtual ~Container();
};

#endif /* Container_hpp */
