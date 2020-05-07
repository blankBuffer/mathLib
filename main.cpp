#include <iostream>

#include "Container.hpp"
#include "Var.hpp"
#include "Const.hpp"
#include "Product.hpp"
#include "Fraction.hpp"
#include "Sum.hpp"

void printM(){
    printf("\n%d\n",Container::conCount);
}

int main() {
    
    char* name = new char[1];
    name[0] = 'x';
    Var* x = new Var(name,1);
    
    Container** factor1List = new Container*[2];
    factor1List[0] = x;
    factor1List[1] = new Const(1);
    Container* factor1 = new Sum(factor1List,2);
    
    Container** factor2List = new Container*[2];
    factor2List[0] = x;
    factor2List[1] = new Const(5);
    Container* factor2 = new Sum(factor2List,2);
    
    Container** factor3List = new Container*[2];
    factor3List[0] = x;
    factor3List[1] = new Const(7);
    Container* factor3 = new Sum(factor3List,2);
    
    Container** numList = new Container*[2];
    numList[0] = factor1;
    numList[1] = factor2;
    
    Container* num = new Product(numList,2);
    
    Container** denList = new Container*[2];
    denList[0] = factor3;
    denList[1] = factor1;
    
    Container* den = new Product(denList,2);
    
    Container* frac = new Fraction(num,den);
    
    printM();
    Container* simp = frac->copy();
    simp->print();
    for(int i = 0;i<10;i++){
        std::cin.ignore();
        Container* old = simp;
        simp=simp->eval();
        delete old;
        simp->print();
    }
    delete simp;
    printM();
    frac->print();
    
}
