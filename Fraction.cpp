//
//  Fraction.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Fraction.hpp"
#include "Const.hpp"
#include "Product.hpp"
#include <climits>

Fraction::Fraction(Container* n,Container* d){
    numerator = n;
    denominator = d;
    type = FRAC;
}
void Fraction::print(){
    printf("(");
    numerator->print();
    printf("/");
    denominator->print();
    printf(")");
}
Container* Fraction::copy(){
    return new Fraction(numerator->copy(),denominator->copy());
}
Container* Fraction::eval(){
    //eval fraction parts
    Container* numerator = this->numerator->eval();
    Container* denominator = this->denominator->eval();
    
    if(numerator->equalStruct(denominator)){
        printf("\nnumerator same as denominator\n");
        delete numerator;
        delete denominator;
        return new Const(1);
    }
    //  2/4 -> 1/2
    if(numerator->type==CONST && denominator->type==CONST){
        printf("\ntrying to reduce fraction\n");
        int n = ((Const*)numerator)->value;
        int d = ((Const*)denominator)->value;
        int min;
        if(n<d) min = n;
        else min = d;
        for(int i = min;i>1;i--){
            if(n%i==0&&d%i==0){
                n/=i;
                d/=i;
                break;
            }
        }
        delete numerator;
        delete denominator;
        return new Fraction(new Const(n),new Const(d));
    }
    //if numerator and denominator are just products
    if(numerator->type==PROD && denominator->type==PROD){
        printf("\ntrying to cancel factors\n");
        //find equal structures and remove them
        Product* num = (Product*)numerator;
        Product* den = (Product*)denominator;
        int max;
        if(num->containersLength > den->containersLength) max = num->containersLength;
        else max = den->containersLength;
        int listOfDuplicates[max];
        for(int i = 0;i<max;i++) listOfDuplicates[i] = INT_MIN;
        int count = 0;
        for(int i = 0;i<num->containersLength;i++){
            for(int j = 0;j<den->containersLength;j++){
                if(num->containers[i]->equalStruct(den->containers[j])){
                    listOfDuplicates[i] = j;
                    count++;
                }
            }
        }
        //make new fraction
        Container** numList = new Container*[num->containersLength-1-count];
        Container** denList = new Container*[den->containersLength-1-count];
        Product* numProd = new Product(numList,num->containersLength-count);
        Product* denProd = new Product(denList,den->containersLength-count);
        int curIndex = 0;
        for(int i = 0;i<num->containersLength;i++){
            if(listOfDuplicates[i]!=INT_MIN) continue;
            numList[curIndex] = num->containers[i]->copy();
            curIndex++;
        }
        curIndex = 0;
        for(int i = 0;i<den->containersLength;i++){
            bool check = false;
            for(int j = 0;j<max;j++) if(listOfDuplicates[j]==i) check = true;
            if(check) continue;
            denList[curIndex] = den->containers[i]->copy();
            curIndex++;
        }
        delete numerator;
        delete denominator;
        return new Fraction(numProd,denProd);
    }
    //check if numerator product has same container as denominator
    if(numerator->type==PROD){
        
    }
    //check if denominator product has same container as numerator
    
    //check if fraction in numertor and fraction in denominator
    
    
    return new Fraction(numerator,denominator);
}
bool Fraction::equalStruct(Container* c){
    if(c->type == FRAC){
        Fraction* other = (Fraction*)c;
        if(other->numerator->equalStruct(numerator) && other->denominator->equalStruct(denominator)){
            return true;
        }
        
    }
    return false;
}
Fraction::~Fraction(){
    delete numerator;
    delete denominator;
}
