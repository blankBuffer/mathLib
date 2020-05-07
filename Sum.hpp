//
//  Sum.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
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
    virtual Container* copy();
    virtual Container* eval();
    virtual bool equalStruct(Container* c);
    ~Sum();
};

#endif /* Sum_hpp */
