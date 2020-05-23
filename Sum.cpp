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
    printf("(");
    for(int i = 0;i<containersLength-1;i++){
        containers[i]->print();
        printf("+");
    }
    if(containersLength>0) containers[containersLength-1]->print();
    printf(")");
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
    if(Container::printSteps) printf("\nadding constants together\n");
    long num = 0L;
    long den = 1L;
    int count = 0;
    bool indexOfR[currentSum->containersLength];
    for(int i = 0;i<currentSum->containersLength;i++) indexOfR[i] = false;
    for(int i = 0;i<currentSum->containersLength;i++){
        Container* currentObj = currentSum->containers[i];
        //classify
        if(currentObj->type == CONST){
            indexOfR[i] = true;
            num+= (((Const*)currentObj)->value)*den;
            fastFracReduc(num,den,&num,&den);
            count++;
            continue;
        }
        //inverse constant
        if(currentObj->type == POW){
            Power* tempP = (Power*)currentObj;
            if(tempP->base->type == CONST && tempP->expo->type == CONST){
                if(((Const*)(tempP->expo))->value == -1){
                    indexOfR[i] = true;
                    long d = ((Const*)(tempP->base))->value;
                    num = num*d+den;
                    den*=d;
                    fastFracReduc(num,den,&num,&den);
                    count++;
                    continue;
                }
            }
        }
        //fraction
        if(currentObj->type == PROD){
            Product* tempP = (Product*)currentObj;
            //if length 2
            if(tempP->containersLength == 2){
                bool case1 = tempP->containers[0]->type == CONST && tempP->containers[1]->type == POW;
                bool case2 = tempP->containers[1]->type == CONST && tempP->containers[0]->type == POW;
                Power* tempPPower = 0;
                Const* tempPConst = 0;
                if(case1){
                    tempPConst = (Const*)tempP->containers[0];
                    tempPPower = (Power*)tempP->containers[1];
                }else if(case2){
                    tempPConst = (Const*)tempP->containers[1];
                    tempPPower = (Power*)tempP->containers[0];
                }
                if(case1||case2){
                    if(tempPPower->base->type == CONST && tempPPower->expo->type == CONST){
                        if(  ((Const*)tempPPower->expo)->value == -1  ){
                        
                            long num2 = tempPConst->value;
                            long den2 = ((Const*)tempPPower->base)->value;
                        
                            num = num2*den+num*den2;
                            den*=den2;
                        
                            indexOfR[i] = true;
                            fastFracReduc(num,den,&num,&den);
                            count++;
                            continue;
                        
                        }
                    }
                }
            }
        }
    }
    if(count < 2) return current;
    
    if(den == 1L){
        modList(currentSum,indexOfR,currentSum->containersLength-count+1);
        currentSum->containers[ currentSum->containersLength-1] = new Const(num);
        return current;
    }
    
    modList(currentSum,indexOfR,currentSum->containersLength-count+1);
    Container** fracList = new Container*[2];
    fracList[0] = new Const(num);
    fracList[1] = new Power( new Const(den) ,new Const(-1) );
    Product* frac = new Product(fracList,2);
    Container* fracEval = frac->eval();
    delete frac;
    currentSum->containers[ currentSum->containersLength-1] = fracEval;
    return current;
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

Container* Sum::combineContainers(Container* current){
    if(current->type!=SUM) return current;
    Sum* currentSum = (Sum*)current;
    if(!current->containsVars()) return current;
    
    if(Container::printSteps) printf("\ntrying to combine variables\n");

    Container** newSum = new Container*[currentSum->containersLength];
    int newSumIndex = 0;
    
    bool usedIndex[currentSum->containersLength];
    for(int i = 0;i<currentSum->containersLength;i++) usedIndex[i] = false;
    
    //remove any parts in product without variables
    //store sum in a list
    
    for(int i = 0;i<currentSum->containersLength;i++){
        if(usedIndex[i]) continue;
        Container** sumList = new Container*[currentSum->containersLength-i];
        int sumListIndex = 0;
        
        Container* currentContainer = currentSum->containers[i]->copy();
        //extract out non var Types
        if(currentContainer->type == PROD){
            Product* currentContainerPCast = (Product*)currentContainer;
            
            bool indexOfC[currentContainerPCast->containersLength];
            int numberOfC = 0;
            for(int j = 0;j<currentContainerPCast->containersLength;j++) {
                if(!currentContainerPCast->containers[j]->containsVars() ){
                    indexOfC[j] = true;
                    numberOfC++;
                }else indexOfC[j] = false;
            }
            Container** subProdList = new Container*[numberOfC];
            int currentSubProdIndex = 0;
            for(int j = 0;j<currentContainerPCast->containersLength;j++){
                if(indexOfC[j]){
                    subProdList[currentSubProdIndex] = currentContainerPCast->containers[j]->copy();
                    currentSubProdIndex++;
                }
            }
            Product* subProd = new Product(subProdList,numberOfC);
            
            //add to sumList
            sumList[sumListIndex] = subProd;
            sumListIndex++;
            
            currentContainerPCast->modList(currentContainerPCast,indexOfC,currentContainerPCast->containersLength-numberOfC);
            Container* currentContainerPCastEval = currentContainerPCast->eval();
            delete currentContainerPCast;
            currentContainer = currentContainerPCastEval;
            
            
        }else{
            sumList[sumListIndex] = new Const(1);
            sumListIndex++;
        }
        //
        //extrated non vars out of currentContainer
        //
        for(int n = i+1;n<currentSum->containersLength;n++){
            if(usedIndex[n]) continue;
            Container* compare = currentSum->containers[n];
            if(currentContainer->equalStruct(compare)){
                sumList[sumListIndex] = new Const(1);
                sumListIndex++;
                usedIndex[n] = true;
            }else{
                //seperate compare
                if(compare->type == PROD){
                    Product* compareProd = (Product*)compare->copy();
                    bool indexOfC[compareProd->containersLength];
                    int numberOfC = 0;
                    for(int j = 0;j<compareProd->containersLength;j++){
                        if(!compareProd->containers[j]->containsVars()){
                            indexOfC[j] = true;
                            numberOfC++;
                        }else indexOfC[j] = false;
                    }
                    Container** prodOfConst = new Container*[numberOfC];
                    int indexOfProdOfConst = 0;
                    for(int j = 0;j<compareProd->containersLength;j++){
                        if(indexOfC[j]){
                            prodOfConst[indexOfProdOfConst] = compareProd->containers[j]->copy(); 
                            indexOfProdOfConst++;
                        }
                    }
                    Container* prodOfConstObj = new Product(prodOfConst,numberOfC);
                    compareProd->modList(compareProd,indexOfC,compareProd->containersLength-numberOfC);
                    Container* compareProdEval = compareProd->eval();
                    delete compareProd;
                    compare = compareProdEval;
                    if(currentContainer->equalStruct(compare)){
                        sumList[sumListIndex] = prodOfConstObj;
                        sumListIndex++;
                        usedIndex[n] = true;
                    }else{
                        delete prodOfConstObj;
                    }
                    delete compare;
                }
            }
        }
        
        Container** reducedSumList = new Container*[sumListIndex];
        for(int m = 0;m<sumListIndex;m++) reducedSumList[m] = sumList[m];
        delete[] sumList;
        
        Sum* sumObj = new Sum(reducedSumList,sumListIndex);
        
        
        Container** newProdList = new Container*[2];
        newProdList[0] = sumObj;
        newProdList[1] = currentContainer->copy();
        delete currentContainer;
        Product* newProd = new Product(newProdList,2);
        newSum[newSumIndex] = newProd;
        newSumIndex++;
        
    }
    Container** newSumResize = new Container*[newSumIndex];
    for(int m = 0;m < newSumIndex;m++){
        newSumResize[m] = newSum[m]->eval();
        delete newSum[m];
    }
    delete[] newSum;
    
    delete currentSum;
    return new Sum(newSumResize,newSumIndex);
}

Container* Sum::removeZeros(Container* current){
    if(current->type!=SUM) return current;
    Sum* currentSum = (Sum*)current;
    
    bool indexOfR[currentSum->containersLength];
    int count = 0;
    
    for(int i = 0;i<currentSum->containersLength;i++){
        indexOfR[i] = false;
        if(currentSum->containers[i]->type == CONST){
            if(((Const*)currentSum->containers[i])->value == 0){
                indexOfR[i] = true;
                count++;
            }
        }
    }
    
    if(count == 0) return current;
    
    modList(currentSum,indexOfR,currentSum->containersLength-count);
    
    return current;
}

Container* Sum::eval(){
    if(Container::printSteps){
        printf("\n");
        this->print();
        printf("\n");
    }
    if(Container::printSteps) printf("\nevaluating sum\n");
    Container** containers = new Container*[containersLength];
    for(int i = 0;i<containersLength;i++) containers[i] = this->containers[i]->eval();
    Container* current = new Sum(containers,containersLength);
    
    current = convertToSingleSumList(current);// (a+(b+c))->(a+b+c)
    current = combinedConstants(current);//adds fractions whole numbers and inverse constants together
    current = combineContainers(current);// (x+3*x)->4*x
    current = removeZeros(current);//0+x -> x
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
