#include <stdio.h>
#include <time.h> 

#include "ConTypes.hpp"

void printM(){
    printf("\nContainers in memory: %d\n",Container::conCount);
}

Container* createStruct(){
    while (true) {
        printf("container type 0:Constant, 1:Variable, 2:Product, 3:Power, 4:Sum, 5:Log\n");
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
        }else if(type == Container::SUM){
            int len = 0;
            printf("length of sum\n");
            scanf("%d",&len);
            Container** list = new Container*[len];
            for(int i = 0;i<len;i++){
                list[i] = createStruct();
            }
            Container* sum = new Sum(list,len);
            return sum;
        }else if(type == Container::LOG){
            printf("describe base\n");
            Container* base = createStruct();
            printf("describe expression\n");
            Container* expr = createStruct();
            return new Log(base,expr);
        }
    }
}

Truth* createTruth(){
    printf("\ntype the left side\n");
    Container* l = createStruct();
    printf("\ntype the right side\n");
    Container* r = createStruct();
    return new Truth(l,r);
}

void simpleUserProg(){
    printM();
    Truth* t = createTruth();
    printf("----------------------------\n");
    t->print();
    //time to calculate
    
    clock_t time; 
    time = clock(); 
    Truth* e = t->eval();
    time = clock() - time; 
    int time_taken = (int)(((double)time)/CLOCKS_PER_SEC*1000000.0);
    printf("\nIt took %d micro seconds\n", time_taken); 
    printf("\n----------------------------\n");
    e->print();
    printf("\n----------------------------\n");
    delete t;
    delete e;
    printM();
}

void askToShowSteps(){
    char c = ' ';
    scanf(" %c",&c);
    if(c=='y'||c=='Y'){
        Container::printSteps = true;
    }
}

int main() {
    
    printf("\nshow steps for debug? y/n\n");
    askToShowSteps();
    
    simpleUserProg();
    return 0;
    
}
