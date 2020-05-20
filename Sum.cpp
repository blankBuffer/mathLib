//
//  Sum.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/14/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Sum.hpp"
#include "Const.hpp"
#include "Power.hpp"
#include "Product.hpp"

Sum::Sum(Container** containers,int containersLength){
    this->containers = containers;
    this->containersLength = containersLength;
    type = SUM;
}

void Sum::print(){
    printf("{");
    for(int i = 0;i<containersLength-1;i++){
        containers[i]->print();
        printf("+");
    }
    if(containersLength>0) containers[containersLength-1]->print();
    printf("}");
}

Container* Sum::copy(){
    Container** list = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) list[i] = containers[i]->copy();
    return new Sum(list,containersLength);
}

Container* Sum::convertToSingleSumList(Container* current){
    if(current->type!=SUM) return current;
    Sum* currentSum = (Sum*)current;
    int total = 0;
    bool indexOfSum[currentSum->containersLength];
    for(int i = 0;i<currentSum->containersLength;i++){
        if(currentSum->containers[i]->type==SUM){
            Sum* subSum = (Sum*)currentSum->containers[i];
            total+=subSum->containersLength;
            indexOfSum[i] = true;
        }else{
            total+=1;
            indexOfSum[i] = false;
        }
    }
    if(total == currentSum->containersLength) return current;
    //begin merge
    if(Container::printSteps) printf("\nsum contains sum : %d elements\n",total);
    int currentIndex = 0;
    Container** list = new Container*[total];
    for(int i = 0;i<currentSum->containersLength;i++){
        if(indexOfSum[i]){
            Sum* subSum = (Sum*)currentSum->containers[i];
            for(int j = 0;j<subSum->containersLength;j++){
                list[currentIndex] = subSum->containers[j]->copy();
                currentIndex++;
            }
        }else{
            list[currentIndex] = currentSum->containers[i]->copy();
            currentIndex++;
        }
    }
    delete current;
    return new Sum(list,total);
}

Container* Sum::checkIfAlone(Container* current){
    if(current->type!=SUM) return current;
    Sum* currentSum = (Sum*)current;
    if(currentSum->containersLength>1) return current;
    if(Container::printSteps) printf("\nsum alone returning container\n");
    if(currentSum->containersLength==1){
        Container* ret = currentSum->containers[0]->copy();
        delete current;
        return ret;
    }else{
        delete current;
        return new Const(0);
    }
}
Container* Sum::combinedConstants(Container* current){
    if(current->type!=SUM) return current;
    Sum* currentSum = (Sum*)current;
    int constCount = 0;
    for(int i = 0;i<currentSum->containersLength;i++) if(currentSum->containers[i]->type == CONST) constCount++;
    if(constCount < 2) return current;
    if(Container::printSteps) printf("\nadding constants\n");
    bool indexOfR[currentSum->containersLength];
    long theSumOfConst = 0L;
    for(int i = 0;i<currentSum->containersLength;i++){
        if(currentSum->containers[i]->type == CONST){
            Const* c = (Const*)currentSum->containers[i];
            theSumOfConst+=c->value;
            indexOfR[i] = true;
        }else indexOfR[i] = false;
    }
    modList(current, indexOfR, currentSum->containersLength-constCount+1);
    currentSum->containers[currentSum->containersLength-1] = new Const(theSumOfConst);
    return currentSum;
}

void Sum::modList(Container* current,bool* indexOfR,int modLength){
    if(current->type!=SUM) return;
    Sum* currentSum = (Sum*)current;
    Container** newList = new Container*[modLength];
    int currentIndex = 0;
    for (int i = 0; i<currentSum->containersLength; i++) {
        if(indexOfR[i]){
            delete currentSum->containers[i];
        }else{
            newList[currentIndex] = currentSum->containers[i];
            currentIndex++;
        }
    }
    delete [] currentSum->containers;
    currentSum->containersLength = modLength;
    currentSum->containers = newList;
}

void Sum::fastFracReduc(long num,long den,long* outNum,long* outDen){
    signed char sig = 1;
    if(num<0){
        sig*=-1;
        num = -num;
    }
    if(den<0){
        sig*=-1;
        den = -den;
    }
    long min;
    if(num>den) min = den;
    else min = num;
    for(long i = min;i>1;i--){
        if(num%i==0&&den%i==0){
            num/=i;
            den/=i;
            break;
        }
    }
    *outNum = num*sig;
    *outDen = den;
}

Container* Sum::combinedIConstants(Container* current){
    if(current->type!=SUM) return current;
    Sum* currentSum = (Sum*)current;
    long num = 0L;
    long den = 1L;
    int count = 0;
    bool indexOfR[currentSum->containersLength];
    for(int i = 0;i<currentSum->containersLength;i++) indexOfR[i] = false;
    
    for (int i = 0; i< currentSum->containersLength; i++) {
        if(currentSum->containers[i]->type == POW){
            Power* pCast = (Power*)currentSum->containers[i];
            if(pCast->base->type == CONST && pCast->expo->type == CONST){
                if(((Const*)pCast->expo)->value == -1){
                    long den2 = ((Const*)pCast->base)->value;
                    long newDen = den*den2;
                    long newNum = num*den2+den;
                    num = newNum;
                    den = newDen;
                    fastFracReduc(num, den, &num, &den);
                    indexOfR[i] = true;
                    count++;
                }
            }
        }
    }
    
    if(count<2) return current;
    if(Container::printSteps) printf("\nadding inverse constants\n");
    modList(currentSum, indexOfR, currentSum->containersLength-count+1);
    
    Container** list = new Container*[2];
    list[0] = new Const(num);
    list[1] = new Power(new Const(den),new Const(-1));
    Product* addedInverse = new Product(list,2);
    Container* evalAddedInverse = addedInverse->eval();
    delete addedInverse;
    currentSum->containers[currentSum->containersLength-1] = evalAddedInverse;
    return current;
}

Container* Sum::eval(){
    if(Container::printSteps) printf("\nevaluating sum\n");
    Container** containers = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) containers[i] = this->containers[i]->eval();
    Container* current = new Sum(containers,containersLength);
    
    current = convertToSingleSumList(current);
    current = combinedConstants(current);
    current = combinedIConstants(current);
    current = checkIfAlone(current);
    
    return current;
}

bool Sum::equalStruct(Container* c){
    //order of containers do not matter
    if(c->type == SUM){
        Sum* other = (Sum*)c;
        if(other->containersLength != this->containersLength) return false;
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

bool Sum::containsVars(){
    bool contains = false;
    for(int i = 0;i<containersLength;i++){
        contains = containers[i]->containsVars();
        if(contains) break;
    }
    return contains;
}
bool Sum::containsContainer(Container* c){
    bool contains = false;
    contains = c->equalStruct(this);
    if(contains) return true;
    for (int i = 0; i<containersLength; i++) {
        contains = containers[i]->containsContainer(c);
        if(contains) return true;
    }
    return false;
}

Sum::~Sum(){
    for(int i = 0;i<containersLength;i++) delete containers[i];
    delete[] containers;
}
