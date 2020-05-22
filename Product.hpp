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
    Container* convertToSingleProductList(Container* current);
    Container* combinedConstants(Container* current);
    Container* combinedIConstants(Container* current);
    Container* checkIfAlone(Container* current);
    Container* removeOne(Container* current);
    Container* deleteIfZero(Container* current);
    Container* simpleFraction(Container* current);
    Container* combineContainers(Container* current);
    Container* distribute(Container* current);
    void modList(Container* current,bool* indexOfR,int modLength);
    bool equalStruct(Container* c);
    bool containsVars();
    bool containsContainer(Container* c);
    ~Product();
};

#endif /* Product_hpp */
