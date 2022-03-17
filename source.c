#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*保留字表*/
static char ReserveWord[32][20] = {
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

/*界符表*/
static char Delimiter[9][3]={
    "{","}","[","]","(",")", ",", ";", ":"
};

/*标识符表*/
static  char Identifier[1000][20] = { "" };

/*判断是否为数字*/
int isDigit(char c){
    if(c>='0'&&c<='9')
        return 1;
    return 0;
}

/*判断是否为字母或下划线*/
int isLetter(char c){
    //C语言允许下划线也为标识符的一部分,可以放在首部或其他地方
    if(c>='a'&&c<='z'||c>='A'&&c<='Z'||c=='_')
        return 1;
    return 0;
}

/*查找保留字*/
int searchReserve(char s[]){
    for (int i = 0; i < 32; i++){
        //若成功查找，则返回种别码
        if (strcmp(ReserveWord[i], s) == 0)
            return i + 4;
    }
    //否则返回0，代表查找不成功，即为标识符
    return 0;
}


/*编译预处理，取出无用的注释和字符*/
void preprocess(char source[],int p){
    char temp[10000]={0};
    int count=0;
    for(int i=0;i<=p;++i){
        //单行注释，则去掉注释后面的内容，直至遇到回车换行
        if(source[i]=='/'&&source[i+1]=='/'){
            while(source[i]!='\n'){
                i++;
            }
        }
        //多行注释，则去掉注释部分的内容
        if(source[i]=='/'&&source[i+1]=='*'){
            i+=2;
            while(!(source[i]=='*'&&source[i+1]=='/')){
                i++;
                if(source[i]=='#'){
                    printf("注释出错，没有找到*/，程序结束\n");
                    exit(-1);
                }
            }
            i+=2;
        }
        if(source[i]!='\n'&&source[i]!='\t'){
            temp[count++]=source[i];
        }
    }
    temp[count]='\0';
    strcpy(source,temp);
}


/*扫描token并返回其种别码，实现有限自动机的功能*/
int scanner(char source[],char token[],int *pos){
    int p=*pos;
    int count=0;
    int state;

    //每次扫描前清零
    for(int i=0;i<20;++i)
        token[i]='\0';

    //过滤空格
    while(source[p]==' ')
        p++;
    
    //开头为字母：保留字或标识符
    if(isLetter(source[p])){
        token[count++]=source[p++];
        while(isLetter(source[p])||isDigit(source[p])){
            token[count++]=source[p++];
        }
        token[count]='\0';
        state=searchReserve(token);//查表找到种别码
    }

    //开头为数字：数字常量
    else if(isDigit(source[p])){
        token[count++]=source[p++];
        while(isDigit(source[p])){ //整型常量
            token[count++]=source[p++];
        }
        if(source[p]=='.'){ //浮点型常量
            do{
                token[count++]=source[p++];
            }while(isDigit(source[p]));
        }
        state=1;
    }

    //开头为'：字符常量
    else if(source[p]=='\''){
        token[count++]='\'';
        token[count++]=source[p+1];
        token[count++]='\'';
        p+=3;
        state=2;
    }

    //开头为": 字符串
    else if(source[p]=='\"'){
        token[count++]=source[p++];
        while(isLetter(source[p])){
            token[count++]=source[p++];
        }
        token[count++]=source[p++];
        token[count]='\0';
        state=3;
    }

    //运算符: + += ++
    else if(source[p]=='+'){
        token[count++]=source[p++];
        if(source[p]=='='||source[p]=='+'){ //+= ++
            token[count++]=source[p++];
            state=source[p-1]=='='?40:44;
        }
        else { //+
            state=36;
        }
    }

    //运算符：- -= --
    else if (source[p]=='-'){
        token[count++]=source[p++];
        if(source[p]=='='||source[p]=='-'){ //-= --
            token[count++]=source[p++];
            state=source[p-1]=='='?41:45;
        }
        else { //-
            state=37;
        }
    }

    //运算符：* *=
    else if(source[p]=='*'){
        token[count++]=source[p++];
        if(source[p]=='='){ //*=
            token[count++]=source[p++];
            state=42;
        }
        else { //*
            state=38;
        }
    }
    
    //运算符：/ /=
    else if(source[p]=='/'){
        token[count++]=source[p++];
        if(source[p]=='='){ // /=
            token[count++]=source[p++];
            state=43;
        }
        else { //  /
            state=39;
        }
    }

    //运算符：= ==
    else if(source[p]=='='){
        token[count++]=source[p++];
        if(source[p]=='='){ // ==
            token[count++]=source[p++];
            state=51;
        }
        else { //  =
            state=50;
        }
    }

    //运算符：< <= <<
    else  if (source[p]=='<'){
        token[count++]=source[p++];
        if(source[p]=='='||source[p]=='<'){ //<= <<
            token[count++]=source[p++];
            state=source[p-1]=='='?49:54;
        }
        else { //<
            state=48;
        }
    }

    //运算符：> >= >>
    else  if (source[p] == '>'){
        token[count++]=source[p++];
        if(source[p]=='='||source[p]=='>'){ //<= <<
            token[count++]=source[p++];
            state=source[p-1]=='='?47:53;
        }
        else { // >
            state=46;
        }
    }

    //运算符： & &&
    else  if (source[p] == '&'){
        token[count++]=source[p++];
        if(source[p]=='&'){ // &&
            token[count++]=source[p++];
            state=56;
        }
        else { //  &
            state=55;
        }
    }

    //运算符： |  ||
    else  if (source[p] == '|'){
        token[count++]=source[p++];
        if(source[p]=='|'){ // ||
            token[count++]=source[p++];
            state=58;
        }
        else { //  |
            state=57;
        }
    }

    //运算符：！=
    else  if (source[p] == '!'&&source[p+1]=='='){
        token[count++]=source[p++];
        token[count++]=source[p++];
        state=52;
    }

    //运算符：%
    else  if (source[p] == '%'){
        token[count++]=source[p++];
        state=59;
    }

    //界符
    else if(source[p]=='{'||source[p]=='}'||source[p]=='['||source[p]==']'||source[p]=='('
    ||source[p]==')'||source[p]==','||source[p]==';'||source[p]==':'){
        token[0]=source[p++];
        for(int i=0;i<9;++i){//查找界符表
            if(strcmp(token,Delimiter[i])==0){
                state=i+60;//获得种别码
                break; //查到即退出
            }
        }
    }

    //程序结束
    else if(source[p]=='#'){
        token[count++]=source[p++];
        state=-1;
    }

    //不能被以上词法分析识别，则出错
    else{
        printf("error:there is no exist %c \n", source[p]);
        exit(-1);
    }    

    *pos=p;
    return state;
}


int main(){
    char source[10000];
    char token[20]={0};
    int p=0; //源程序指针
    FILE *fp_input,*fp_output;

    //打开输入文件
    if((fp_input = fopen("demo.c", "r")) == NULL){ 
        printf("can not open demo.c!\n");
        exit(0);
    }

    //将源程序读入source[]数组
    source[p]=fgetc(fp_input);
    while(source[p]!='#')
        source[++p]=fgetc(fp_input);
    source[++p]='\0';
    fclose(fp_input);
    /*printf("\nSource code:\n");
    for(int i=0;i<p;++i)
        printf("%c",source[i]);*/

    //预处理
    preprocess(source,p);
    /*printf("\nAfter preprocessing:\n");
    for(int i=0;i<p;++i) 
        printf("%c",source[i]);
    printf("\n");*/

    //打开输出文件
    if ((fp_output = fopen("tokens.txt", "w+")) == NULL){
        printf("can't open tokens.txt!\n");
        exit(0);
    }

    p=0;//从头开始分析
    int state=0; //设置初始状态

    while(state!=-1){
        state=scanner(source,token,&p);
        fprintf(fp_output,"<%s,%d>\n",token,state);
        //printf("(%s,%d)\n",token,state);
    }
    fclose(fp_output);

}