//
//  Product.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "Container.hpp"
#include "Product.hpp"
#include "Const.hpp"
#include "Power.hpp"

Product::Product(Container** containers,int containersLength){
    this->containers = containers;
    this->containersLength = containersLength;
    type = PROD;
}
void Product::print(){
    printf("{");
    for(int i = 0;i<containersLength-1;i++){
        containers[i]->print();
        printf("*");
    }
    if(containersLength>0) containers[containersLength-1]->print();
    printf("}");
}
Container* Product::copy(){
    Container** list = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) list[i] = containers[i]->copy();
    return new Product(list,containersLength);
}

Container* Product::convertToSingleProductList(Container* current){
    if(current->type!=PROD) return current;
    Product* currentProd = (Product*)current;
    int total = 0;
    bool indexOfProduct[currentProd->containersLength];
    for(int i = 0;i<currentProd->containersLength;i++){
        if(currentProd->containers[i]->type==PROD){
            Product* subProduct = (Product*)currentProd->containers[i];
            total+=subProduct->containersLength;
            indexOfProduct[i] = true;
        }else{
            total+=1;
            indexOfProduct[i] = false;
        }
    }
    if(total == currentProd->containersLength) return current;
    //begin merge
    if(Container::printSteps) printf("\nproduct contains product : %d elements\n",total);
    int currentIndex = 0;
    Container** list = new Container*[total];
    for(int i = 0;i<currentProd->containersLength;i++){
        if(indexOfProduct[i]){
            Product* subProduct = (Product*)currentProd->containers[i];
            for(int j = 0;j<subProduct->containersLength;j++){
                list[currentIndex] = subProduct->containers[j]->copy();
                currentIndex++;
            }
        }else{
            list[currentIndex] = currentProd->containers[i]->copy();
            currentIndex++;
        }
    }
    delete current;
    return new Product(list,total);
}
Container* Product::combinedConstants(Container* current){
    if(current->type!=PROD) return current;
    Product* currentProd = (Product*)current;
    bool indexOfConst[currentProd->containersLength];
    int numberOfConst = 0;
    for(int i = 0;i<currentProd->containersLength;i++){
        if(currentProd->containers[i]->type == CONST){
            indexOfConst[i] = true;
            numberOfConst++;
        }else indexOfConst[i] = false;
    }
    if(numberOfConst < 2) return current;
    //start
    if(Container::printSteps) printf("\ncombining constants\n");
    int len = currentProd->containersLength-numberOfConst+1;
    long value = 1L;
    for(int i = 0;i<currentProd->containersLength;i++){
        if(indexOfConst[i]){
            Const* c = (Const*)currentProd->containers[i];
            value*=c->value;
        }
    }
    modList(current, indexOfConst, len);
    if(len>0){
        currentProd->containers[len-1] = new Const(value);
    }
    return current;
}
Container* Product::combinedIConstants(Container* current){
    if(current->type!=PROD) return current;
    Product* currentProd = (Product*)current;
    bool indexOfI[currentProd->containersLength];
    int numberOfI = 0;
    for(int i = 0;i<currentProd->containersLength;i++){
        if(currentProd->containers[i]->type == POW){
            Power* currentPow = (Power*)currentProd->containers[i];
            if(currentPow->base->type == CONST && currentPow->expo->type == CONST){
                Const* ex = (Const*)(currentPow->expo);
                if(ex->value == -1){
                    indexOfI[i] = true;
                    numberOfI++;
                }else indexOfI[i] = false;
            }else indexOfI[i] = false;
        }else indexOfI[i] = false;
    }
    if(numberOfI < 2) return current;
    //start
    if(Container::printSteps) printf("\ncombining inverse constants\n");
    int len = currentProd->containersLength-numberOfI+1;
    long value = 1L;
    for(int i = 0;i<currentProd->containersLength;i++){
        if(indexOfI[i]){
            Power* p = (Power*)currentProd->containers[i];
            Const* c = (Const*)p->base;
            value*=c->value;
        }
    }
    modList(current, indexOfI, len);
    if(len>0){
        currentProd->containers[len-1] = new Power(new Const(value),new Const(-1L));
    }
    return current;
}
Container* Product::checkIfAlone(Container* current){
    if(current->type!=PROD) return current;
    Product* currentProd = (Product*)current;
    if(currentProd->containersLength>1) return current;
    if(Container::printSteps) printf("\nproduct alone returning container\n");
    if(currentProd->containersLength==1){
        Container* ret = currentProd->containers[0]->copy();
        delete current;
        return ret;
    }else{
        delete current;
        return new Const(1);
    }
}
Container* Product::removeOne(Container* current){
    if(current->type!=PROD) return current;
    Product* currentProd = (Product*)current;
    int count = 0;
    int indexOfOne = -1;
    int indexOfIOne = -1;
    
    for(int i = 0;i<currentProd->containersLength;i++){
        if(indexOfOne == -1 || indexOfIOne == -1){
            if(indexOfOne == -1&&currentProd->containers[i]->type==CONST){
                Const* c = (Const*)currentProd->containers[i];
                if(c->value == 1){
                    indexOfOne = i;
                    count++;
                }
            }else if(indexOfIOne == -1&&currentProd->containers[i]->type==POW){
                Power* p = (Power*)currentProd->containers[i];
                if(p->base->type == CONST && p->expo->type == CONST){
                    Const* b = (Const*)p->base;
                    Const* e = (Const*)p->expo;
                    if(b->value == 1 && e->value == -1){
                        indexOfIOne = i;
                        count++;
                    }
                }
            }
        }else break;
    }
    if(indexOfIOne == -1 && indexOfOne == -1) return current;
    //start
    if(Container::printSteps) printf("\nremoving ones\n");
    int len = currentProd->containersLength-count;
    bool ind[currentProd->containersLength];
    for(int i = 0;i<currentProd->containersLength;i++){
        if(i==indexOfOne || i == indexOfIOne) ind[i] = true;
        else ind[i] = false;
    }
    modList(current, ind, len);
    return current;
}
Container* Product::deleteIfZero(Container* current){
    if(current->type!=PROD) return current;
    Product* currentProd = (Product*)current;
    for(int i = 0;i<currentProd->containersLength;i++){
        if(currentProd->containers[i]->type == CONST){
            Const* c = (Const*)currentProd->containers[i];
            if(c->value == 0){
                if(Container::printSteps) printf("\nzero deletes itself\n");
                delete current;
                return new Const(0);
            }
        }
    }
    return current;
}
Container* Product::simpleFraction(Container* current){
    if(current->type!=PROD) return current;
    Product* currentProd = (Product*)current;
    int indexOfConst = -1;
    int indexOfPower = -1;
    Const* c = 0;
    Power* p = 0;
    for(int i = 0;i < currentProd->containersLength;i++){
        if(indexOfConst == -1 || indexOfPower == -1){
            
            if(currentProd->containers[i]->type == CONST && indexOfConst == -1){
                c = (Const*)currentProd->containers[i];
                indexOfConst = i;
            }else if(currentProd->containers[i]->type == POW && indexOfPower == -1){
                p = (Power*)currentProd->containers[i];
                if(p->base->type == CONST && p->expo->type == CONST){
                    Const* temp = (Const*)p->expo;
                    if(temp->value == -1){
                        indexOfPower = i;
                    }
                }
            }
        }else break;
    }
    if(indexOfConst == -1 || indexOfPower == -1) return current;
    //start
    if(Container::printSteps) printf("\nsimple fraction\n");
    long num = c->value;
    Const* de = (Const*)(p->base);
    long den = de->value;
    
    long sign = 1;
    if(num < 0 != den<0){
        sign = -1;
        if(num<0) num = -num;
        else den = -den;
    }
    
    long min;
    if(num<den) min = num;
    else min = den;
    
    for(long i = min;i>0;i--){
        if(num%i==0 && den%i==0){
            num/=i;
            den/=i;
        }
    }
    
    num*=sign;
    
    delete currentProd->containers[indexOfConst];
    delete currentProd->containers[indexOfPower];
    currentProd->containers[indexOfConst] = new Const(num);
    currentProd->containers[indexOfPower] = new Power(new Const(den),new Const(-1L));
    return current;
}
void Product::modList(Container* current,bool* indexOfR,int modLength){
    if(current->type!=PROD) return;
    Product* currentProd = (Product*)current;
    Container** newList = new Container*[modLength];
    int currentIndex = 0;
    for (int i = 0; i<currentProd->containersLength; i++) {
        if(indexOfR[i]){
            delete currentProd->containers[i];
        }else{
            newList[currentIndex] = currentProd->containers[i];
            currentIndex++;
        }
    }
    delete [] currentProd->containers;
    currentProd->containersLength = modLength;
    currentProd->containers = newList;
}

Container* Product::combineConstantExponents(Container* current){
    //keep list of duplicates after the first
    //duplicate base or container
    //keep a sum list for all the exponents the default is one
    if(current->type!=PROD) return current;
    Product* currentProd = (Product*)current;
    
    bool usedIndex[currentProd->containersLength];
    int numberOfUni = 0;
    for(int i = 0;i<currentProd->containersLength;i++){
        //count duplicates after this iteration
        for(int j = i+1;j<currentProd->containersLength;j++){
            
        }
    }
    
    
    return current;
}

Container* Product::eval(){
    if(Container::printSteps) printf("\nevaluating product\n");
    //make local containers list with simplified sub components
    Container** containers = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) containers[i] = this->containers[i]->eval();
    Container* current = new Product(containers,containersLength);
    
    current = convertToSingleProductList(current);
    current = deleteIfZero(current);
    current = combinedConstants(current);
    current = combinedIConstants(current);
    current = simpleFraction(current);
    current = combineConstantExponents(current);
    current = removeOne(current);
    current = checkIfAlone(current);
    
    //returing new object after basic simplification
    return current;
}
bool Product::equalStruct(Container* c){
    //order of containers do not matter
    if(c->type == PROD){
        Product* other = (Product*)c;
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
bool Product::containsVars(){
    bool contains = false;
    for(int i = 0;i<containersLength;i++){
        contains = containers[i]->containsVars();
        if(contains) break;
    }
    return contains;
}
bool Product::containsContainer(Container* c){
    bool contains = false;
    contains = c->equalStruct(this);
    if(contains) return true;
    for (int i = 0; i<containersLength; i++) {
        contains = containers[i]->containsContainer(c);
        if(contains) return true;
    }
    return false;
}
Product::~Product(){
    for(int i = 0;i<containersLength;i++) delete containers[i];
    delete[] containers;
}
