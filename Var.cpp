//
//  Var.cpp
//  MathSystem
//
//  Created by Benjamin Currie on 5/6/20.
//  Copyright © 2020 Benjamin Currie. All rights reserved.
//

#include "ConTypes.hpp"

Var::Var(char* name,int nameLength){
    this->name = name;
    this->nameLength = nameLength;
    type = VAR;
}
void Var::print(){
    printf("%s",name);
}
Container* Var::copy(){
    char* name = new char[nameLength];
    for(int i = 0;i<nameLength;i++) name[i] = this->name[i];
    return new Var(name,nameLength);
}
Container* Var::eval(){
    return this->copy();
}
bool Var::equalStruct(Container* c){
    if(c->type == VAR){
        Var* other = (Var*)c;
        if(other->nameLength == this->nameLength){
            bool same = true;
            for(int i = 0;i<nameLength;i++){
                if(other->name[i]!=this->name[i]){
                    same = false;
                    break;
                }
            }
            return same;
        }
    }
    return false;
}
bool Var::containsVars(){
    return true;
}
bool Var::containsContainer(Container* c){
    return c->equalStruct(this);
}
int Var::countVars(Var* v){
    if(v->equalStruct(this)) return 1;
    return 0;
}
Var::~Var(){
    delete [] name;
};
