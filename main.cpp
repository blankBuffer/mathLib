#include <stdio.h>

#include "ConTypes.hpp"

void printM(){
    printf("\nContainers in memory: %d\n",Container::conCount);
}

Container* createStruct(){
    while (true) {
        printf("container type 0:Constant, 1:Variable, 2:Product, 3:Power\n");
        int type = 0;
        scanf("%d",&type);
        if(type == Container::CONST){
            printf("type integer\n");
            long value = 0;
            scanf("%ld",&value);
            return new Const(value);
        }else if(type == Container::VAR){
            char name[16];
            for(int i = 0;i<16;i++) name[i] = '\0';
            printf("type variable name\n");
            scanf("%s",name);
            char* nameH = new char[16];
            for(int i = 0;i<16;i++) nameH[i] = name[i];
            return new Var(nameH,16);
        }else if(type == Container::PROD){
            int len = 0;
            printf("length of product\n");
            scanf("%d",&len);
            Container** list = new Container*[len];
            for(int i = 0;i<len;i++){
                list[i] = createStruct();
            }
            Container* prod = new Product(list,len);
            return prod;
        }else if(type == Container::POW){
            printf("describe base\n");
            Container* base = createStruct();
            printf("describe expo\n");
            Container* expo = createStruct();
            return new Power(base,expo);
        }
    }
}

void simpleUserProg(){
    printM();
    Container* con = createStruct();
    Container::printSteps = true;
    printf("----------------------------\n");
    con->print();
    Container* c = con->eval();
    printf("\n----------------------------\n");
    c->print();
    printf("\n----------------------------\n");
    printf("\ncompare to other container\n");
    Container* comparison = createStruct();
    printf("\n----------------------------\n");
    comparison->print();
    printf("\n----------------------------\n");
    printf("\n%d\n",con->containsContainer(comparison));
    delete comparison;
    delete c;
    delete con;
    printM();
}

int main() {
    simpleUserProg();
    return 0;
    
}
