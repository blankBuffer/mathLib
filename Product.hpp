//
//  Product.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Product_hpp
#define Product_hpp

#include <stdio.h>

struct Product : public Container{
    Container** containers;
    int containersLength;
    Product(Container** containers,int containersLength);
    void print();
    Container* copy();
    Container* eval();
    bool equalStruct(Container* c);
    ~Product();
};

#endif /* Product_hpp */
