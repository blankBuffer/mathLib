//
//  Product.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Container.hpp"
#include "Product.hpp"

Product::Product(Container** containers,int containersLength){
    this->containers = containers;
    this->containersLength = containersLength;
    type = PROD;
}
void Product::print(){
    printf("(");
    for(int i = 0;i<containersLength-1;i++){
        containers[i]->print();
        printf("*");
    }
    containers[containersLength-1]->print();
    printf(")");
}
Container* Product::copy(){
    Container** list = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) list[i] = containers[i]->copy();
    return new Product(list,containersLength);
}
Container* Product::eval(){
    Container* result;
    Container** containers = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) containers[i] = this->containers[i]->eval();
    
    //if length is 1 return the container
    if(containersLength==1){
        printf("\nproduct not a list\n");
        result = containers[0]->copy();
        for(int i = 0;i<containersLength;i++) delete containers[i];
        delete [] containers;
        return result;
    }
    //if a container is a prod
    int total = 0;
    for(int i = 0;i<containersLength;i++){
        if(containers[i]->type==PROD){
            total+=((Product*)containers[i])->containersLength;
        }else total++;
    }
    if(total>containersLength){
        printf("\nproduct contains product\n");
        Container** list = new Container*[total];
        int currentIndex = 0;
        for(int i = 0;i<containersLength;i++){
            if(containers[i]->type==PROD){
                Product* prod = (Product*)containers[i];
                for(int j = 0;j<prod->containersLength;j++){
                    list[currentIndex] = prod->containers[j]->copy();
                    currentIndex++;
                }
            }else{
                list[currentIndex] = containers[i]->copy();
                currentIndex++;
            }
        }
        for(int i = 0;i<containersLength;i++) delete containers[i];
        delete [] containers;
        return new Product(list,total);
    }
    //combind numerical constants
    
    
    return new Product(containers,containersLength);
    return nullptr;
}
bool Product::equalStruct(Container* c){
    if(c->type == PROD){
        Product* other = (Product*)c;
        bool used[other->containersLength];
        for(int i = 0;i<other->containersLength;i++) used[i] = false;
        for(int i = 0;i<containersLength;i++){
            bool found = false;
            for(int j = 0;j<other->containersLength;j++){
                if(other->containers[j]->equalStruct(containers[i]) && !used[j]){
                    found = true;
                    used[j] = true;
                    break;
                }
            }
            
            if(!found) return false;
            
        }
        return true;
    }
    return false;
}
Product::~Product(){
    for(int i = 0;i<containersLength;i++) delete containers[i];
    delete[] containers;
}
