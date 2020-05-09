//
//  Const.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Const_hpp
#define Const_hpp

#include <stdio.h>
#include "Container.hpp"

struct Const: public Container{
    long value;
    Const(long value);
    void print();
    Container* copy();
    Container* eval();
    bool equalStruct(Container* c);
    ~Const();
};

#endif /* Const_hpp */
