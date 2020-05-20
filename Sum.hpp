//
//  Sum.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/14/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Sum_hpp
#define Sum_hpp

#include <stdio.h>
#include "Container.hpp"

struct Sum : public Container{
    Container** containers;
    int containersLength;
    Sum(Container** containers,int containersLength);
    void print();
    Container* copy();
    Container* eval();
    bool equalStruct(Container* c);
    bool containsVars();
    bool containsContainer(Container* c);
    Container* convertToSingleSumList(Container* current);
    Container* checkIfAlone(Container* current);
    Container* combinedConstants(Container* current);
    Container* combinedIConstants(Container* current);
    void fastFracReduc(long num,long den,long* outNum,long* outDen);
    void modList(Container* current,bool* indexOfR,int modLength);
    ~Sum();
};

#endif /* Sum_hpp */
