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
    Container* diff(int n);
    Container* solve(Container* c);
    
    ~Truth();
};

#endif /* Truth_hpp */
