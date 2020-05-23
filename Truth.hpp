//
//  Truth.hpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/14/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#ifndef Truth_hpp
#define Truth_hpp

#include <stdio.h>
#include "Container.hpp"

struct Truth{
    Container* left,* right;
    Truth(Container* left,Container* right);
    Truth* diff(int n);
    Truth* solve(Var* v);
    Truth* eval();
    void print();
    
    ~Truth();
};

#endif /* Truth_hpp */
