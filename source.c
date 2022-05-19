#include <string.h>
#include <stdio.h>

struct TOKEN{
    char t;
    int i;
};

struct QT{
    char op;
    struct TOKEN obj1;
    struct TOKEN obj2;
    struct TOKEN res;
};

struct TOKEN word;
struct TOKEN sem[10];  //语义栈
int sem_num;

struct QT qt[30];  //四元式区
int qt_num=0;   
int t_idx=1;   //四元式下标

char exp[50]; //表达式
char cur;   //当前表达式输入
int idx=0;  //当前表达式输入下标

int D();
int E();
int T();
int F();

void next();
void newt();
void GEQ(char);

int main(){
    printf("please input your expression:  ");
    scanf("%s",exp);
    int len=strlen(exp);
    next();
    D();
    if(cur=='#'&&idx==len){
        for(int i=0;i<qt_num;++i){
            printf("(%d)",i+1);
            if(qt[i].op!='='){
                printf("(%c",qt[i].op);
                if(qt[i].obj1.t!='t')
                    printf(",%c",qt[i].obj1.t);
                else   
                    printf(",%c%d",qt[i].obj1.t,qt[i].obj1.i);
                if(qt[i].obj2.t!='t')
                    printf(",%c",qt[i].obj2.t);
                else   
                    printf(",%c%d",qt[i].obj2.t,qt[i].obj2.i);
                printf(",%c%d)",qt[i].res.t,qt[i].res.i);
            }
            else{
                printf("(%c",qt[i].op);
                printf(",%c%d",qt[i-1].res.t,qt[i-1].res.i);
                printf(",_");
                printf(",%c)",qt[i].obj1.t);
            }
            printf("\n");
        }
        printf("OK!");
    }
    else if(idx==len+1)
        printf("Error: The input does not end with '#'\n");
}

int D(){
    char w;
    E();
    while(cur=='='){
        w=cur;
        next();
        E();
        GEQ(w);
    }
    return 1;
}

int E(){
    char w;
    T();
    while(cur=='+'||cur=='-'){
        w=cur;
        next();
        T();
        GEQ(w);
    }
    return 1;
}

int T(){
    char w;
    F();
    while(cur=='*'||cur=='/'){
        w=cur;
        next();
        F();
        GEQ(w);
    }
    return 1;
}

int F(){
    if(cur=='('){
        next();
        E();
        if(cur!=')'){
            printf("Error: bracket mismatch\n");
            return 0;
        }
    }
    else if(cur>='a'&&cur<='z'){  //PUSH(cur)
        word.t=cur;
        word.i=0;
        sem[++sem_num]=word;
    }
    else{
        printf("Error: unrecognized input symbol %c\n",cur);
        return 0;
    }
    next();
    return 1;
}


void next(){
    cur=exp[idx++];
}

void newt(){
    word.t='t';
    word.i=t_idx++;
}

//生成一个四元式送qt
void GEQ(char w){
    newt();
    qt[qt_num].op=w;
    qt[qt_num].obj1=sem[sem_num-1];
    qt[qt_num].obj2=sem[sem_num];
    qt[qt_num].res=word;
    sem_num--;
    sem_num--;
    sem[++sem_num]=word;
    qt_num++;
}