#include <stdio.h>

#include "Container.hpp"
#include "Var.hpp"
#include "Const.hpp"
#include "Product.hpp"
#include "Fraction.hpp"

Container* fullySimp(Container* in){
    Container* c = in->eval();
    Container* nc = c->eval();
    while(!c->equalStruct(nc)){
        delete c;
        c = nc;
        nc = c->eval();
    }
    delete c;
    return nc;
}

int main() {
    
    Container** list1 = new Container*[4];
    list1[0]=new Const(2);
    list1[1]=new Const(2);
    list1[2]=new Const(3);
    list1[3]=new Const(3);
    Product* p = new Product(list1,4);
    
    Container** list2 = new Container*[4];
    list2[0]=new Const(2);
    list2[1]=new Const(3);
    list2[2]=new Const(2);
    list2[3]=new Const(3);
    Product* p2 = new Product(list2,4);
    
    printf("%d",p->equalStruct(p2));
    
    return 0;
}
