//
//  Fraction.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Fraction_hpp
#define Fraction_hpp
#include "Container.hpp"

#include <stdio.h>

struct Fraction : public Container{
    Container* numerator;
    Container* denominator;
    Fraction(Container* n,Container* d);
    void print();
    Container* copy();
    Container* eval();
    bool equalStruct(Container* c);
    ~Fraction();
};

#endif /* Fraction_hpp */
