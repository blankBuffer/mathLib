//
//  Truth.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/14/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "ConTypes.hpp"

Truth::Truth(Container* left,Container* right){
    this->left = left;
    this->right = right;
    Container::conCount++;
}

void Truth::print(){
    left->print();
    printf("=");
    right->print();
}

Truth* Truth::eval(){
    
    if(Container::printSteps) printf("\nevaluating truth\n");
    
    if(left->type==Container::CONST){
        if( ((Const*)left)->value == 0 ){
            if(Container::printSteps) printf("\nalready in correct position\n");
            return new Truth( new Const(0), right->eval() );
            
        }
    }
    
    Container** sumList = new Container*[2];
    sumList[0] = right->copy();
    
    Container** prodList = new Container*[2];
    prodList[0] = left->copy();
    prodList[1] = new Const(-1);
    
    sumList[1] = new Product(prodList,2);
    
    Sum* allOneSide = new Sum(sumList,2);
    Container* allOneSideEval = allOneSide->eval();
    delete allOneSide;
    return new Truth( new Const(0), allOneSideEval );
}

Truth* Truth::solve(Var* v){
    bool success = false;
    Truth* current = eval();
    if(current->right->countVars(v) == 0){
        if(Container::printSteps) printf("\nvariable not found\n");
        return current;
    }
    //factor technique
    if(current->right->type == Container::PROD){
        if(Container::printSteps) printf("\nsolving a factor\n");
        int indexOfVar = -1;
        Product* p = (Product*)current->right;
        
        int min = 5096;
        
        for(int i = 0;i<p->containersLength;i++){
            int c = p->containers[i]->countVars(v);
            if(c>0){
                if(c<min){
                    indexOfVar = i;
                    min = c;
                }
            }
        }
        
        Truth* fa = new Truth(new Const(0),p->containers[indexOfVar]->copy());
        delete current;
        current = fa;
        
    }
    if(current->right->countVars(v) > 1) if(Container::printSteps) printf("\nunable to isolate variable\n");
    //reorganize sum
    
    if(current->right->type == Container::SUM){
        if(Container::printSteps) printf("\nsubtracting non var parts\n");
        Sum* sm = (Sum*)current->right;
        
        bool containsVar[sm->containersLength];
        int varPartsCount = 0;
        int nonVarPartsCount = 0;
        
        for(int i = 0;i<sm->containersLength;i++){
            if(sm->containers[i]->countVars(v)>0){
                containsVar[i] = true;
                varPartsCount++;
            }else{
                containsVar[i] = false;
                nonVarPartsCount++;
            }
        }
        
        Container** nonVarSum = new Container*[nonVarPartsCount];
        int nonVarSumIndex = 0;
        
        Container** varSum = new Container*[varPartsCount];
        int varSumIndex = 0;
        
        for(int i = 0;i<sm->containersLength;i++){
            if(containsVar[i]){
                varSum[varSumIndex] = sm->containers[i]->copy();
                varSumIndex++;
            }else{
                nonVarSum[nonVarSumIndex] = sm->containers[i]->copy();
                nonVarSumIndex++;
            }
        }
        
        Sum* nonVarSumObj = new Sum(nonVarSum,nonVarPartsCount);
        Sum* varSumObj = new Sum(varSum,varPartsCount);
        
        Container** reverse = new Container*[2];
        reverse[0] = nonVarSumObj;
        reverse[1] = new Const(-1);
        Product* reverseObj = new Product(reverse,2);
        
        Container** newLeftSum = new Container*[2];
        newLeftSum[0] = reverseObj;
        newLeftSum[1] = current->left->copy();
        
        Truth* replace = new Truth(new Sum(newLeftSum,2),varSumObj);
        delete current;
        current = replace;
        
    }
    
    Truth* finalEval = new Truth(current->left->eval(),current->right->eval());
    delete current;
    current = finalEval;
    
    return current;
}

Truth::~Truth(){
    delete left;
    delete right;
    Container::conCount--;
}
