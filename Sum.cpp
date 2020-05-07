//
//  Sum.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Sum.hpp"
#include "Const.hpp"

Sum::Sum(Container** containers,int containersLength){
    this->containers = containers;
    this->containersLength = containersLength;
    type = SUM;
}
void Sum::print(){
    printf("(");
    for(int i = 0;i<containersLength-1;i++){
        containers[i]->print();
        printf("+");
    }
    containers[containersLength-1]->print();
    printf(")");
}
Container* Sum::copy(){
    Container** list = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) list[i] = containers[i]->copy();
    return new Sum(list,containersLength);
}
Container* Sum::eval(){
    Container* result;
    Container** containers = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) containers[i] = this->containers[i]->eval();
    
    //if length is 1 return the container
    if(containersLength==1){
        printf("\nsum not a list\n");
        result = containers[0]->copy();
        for(int i = 0;i<containersLength;i++) delete containers[i];
        delete [] containers;
        return result;
    }
    //if a container is a sum
    int total = 0;
    for(int i = 0;i<containersLength;i++){
        if(containers[i]->type==SUM){
            total+=((Sum*)containers[i])->containersLength;
        }else total++;
    }
    if(total>containersLength){
        printf("\nproduct contains product\n");
        Container** list = new Container*[total];
        int currentIndex = 0;
        for(int i = 0;i<containersLength;i++){
            if(containers[i]->type==SUM){
                Sum* sum = (Sum*)containers[i];
                for(int j = 0;j<sum->containersLength;j++){
                    list[currentIndex] = sum->containers[j]->copy();
                    currentIndex++;
                }
            }else{
                list[currentIndex] = containers[i]->copy();
                currentIndex++;
            }
        }
        for(int i = 0;i<containersLength;i++) delete containers[i];
        delete [] containers;
        return new Sum(list,total);
    }
    //combining numbers as sum
    bool isNum[containersLength];
    for(int i = 0;i<containersLength;i++) isNum[i] = false;
    int count = 0;
    for(int i = 0;i<containersLength;i++){
        if(containers[i]->type == CONST){
            Const* c = (Const*)(containers[i]);
            if(!c->isContainer){
                count++;
                isNum[i] = true;
            }
        }
    }
    if(count>1){
        printf("\ncombining constants in sum\n");
        int newLength = containersLength-count+1;
        Const* combin = new Const(0L);
        Container** newList = new Container*[newLength-1];
        int currentIndex = 0;
        for(int i = 0;i<containersLength;i++){
            if(!isNum[i]){
                newList[currentIndex] = containers[i]->copy();
                currentIndex++;
            }else{
                combin->value+=((Const*)containers[i])->value;
            }
        }
        newList[newLength-1] = combin;
        for(int i = 0;i<containersLength;i++) delete containers[i];
        delete [] containers;
        return new Sum(newList,newLength);
    }
    //combine a fractional pair if exists
    
    
    
    //if one constant and one fraction in list combine them
    
    return new Sum(containers,containersLength);
}
bool Sum::equalStruct(Container* c){
    if(c->type == SUM){
        Sum* other = (Sum*)c;
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
Sum::~Sum(){
    for(int i = 0;i<containersLength;i++) delete containers[i];
    delete[] containers;
}
