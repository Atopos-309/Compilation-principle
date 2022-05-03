#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
#include<stack>
#include<string>
using namespace std;

//表格数组                +       -        *       /       i      （      ）       # 	    E	    T		 F
char LR0[50][50][100] = {{" " ," " ," ", " " ,"S4"   ,"S5"   ," " ," " ,"1"    ,"2"    ,"3"   },//   0
                         {"S6"   ,"S7"   ," ", " " ,"  "," " ," " ,"acc"  ," " ," " ," "},//   1
                         {"r3"   ,"r3"   ,"S8"  , "S9"   ," " ," " ,"r3"   ,"r3"   ," " ," " ," "},//   2
                         {"r6"   ,"r6"   ,"r6"  , "r6"   ," " ," " ,"r6"   ,"r6"   ," " ," " ," "},//   3
                         {"r8"   ,"r8"   ,"r8"  , "r8"   ," " ," " ,"r8"   ,"r8"   ," " ," " ," "},//   4
                         {" " ," " ," ", " " ,"S4"   ,"S5"   ," " ," " ,"10"   ,"2"    ,"3"   },//   5
                         {" " ," " ," ", " " ,"S4"   ,"S5"   ," " ," " ," " ,"11"   ,"3"   },//   6
                         {" " ," " ," ", " " ,"S4"   ,"S5"   ," " ," " ," " ,"12"   ,"3"   },//   7
                         {" " ," " ," ", " " ,"S4"   ,"S5"   ," " ," " ," " ," " ,"13"  },//   8
                         {" " ," " ," ", " " ,"S4"   ,"S5"   ," " ," " ," " ," " ,"14"  },//   9
                         {"S6"   ,"S7"   ," ", " " ," " ," " ,"S15"  ," " ," " ," " ," "},//   10
                         {"r1"   ,"r1"   ,"S8"  , "S9"   ," " ," " ,"r1"   ,"r1"   ," " ," " ," "},//   11
                         {"r2"   ,"r2"   ,"S8"  , "S9"   ," " ," " ,"r2"   ,"r2"   ," " ," " ," "},//   12
                         {"r4"   ,"r4"   ,"r4"  , "r4"   ," " ," " ,"r4"   ,"r4"   ," " ," " ," "},//   13
                         {"r5"   ,"r5"   ,"r5"  , "r5"   ," " ," " ,"r5"   ,"r5"   ," " ," " ," "},//   14
                         {"r7"   ,"r7"   ,"r7"  , "r7"   ," " ," " ,"r7"   ,"r7"   ," " ," " ," "},//   15
                          };
char L[200]="+-*/i()#ETF";    //列判断依据
int  del[10]={0,3,3,1,3,3,1,3,1};//0-6号文法每个文法长度
char head[20]={'S','E','E','E','T','T','T','F','F'};
stack<int>Status;    //状态栈
stack<char>Symbol;   //符号栈
char status_arr[300]="0";//初始状态栈对应输出数组
int status_ind = 0;
char symbol_arr[300]="#";//初始符号栈对应输出数组
int symbol_ind = 0;

int findL(char b){//对应列寻找
    for(int i = 0; i <= 10; i++){
        if(b==L[i]){
            return i;
        }
    }
    return -1;
}
void error(int x, int y) {      //报错输出
    printf("Row'%d' Col'%c' is empty!",x,L[y]);
}
 
int calculate(int l, char s[]){
    int num = 0;
    for(int i = 1; i < l; i ++){
        num =  num*10+(s[i]-'0');
    }
    return num;
}

void analyze(char str[],int len){//分析主体过程
    cout<<endl<<"============================Parsing syntax using LR(0)============================"<<endl;
 
    int cnt = 1;

    printf("Step      Status Stack    Symbol Stack    Input    ACTION    GOTO\n");
    int LR = 0;
    while(LR<=len){
        printf("(%d)		%-10s	%-10s",cnt,status_arr,symbol_arr);//步骤，状态栈，符号栈输出
        cnt++;
        for(int i = LR; i < len; i++){//输入串输出
            printf("%c",str[i]);
        }
        for(int i = len-LR; i<10;i++) printf(" ");
 
        int x = Status.top();//状态栈栈顶
        int y = findL(str[LR]);//待判断串串首
 
        if(strcmp(LR0[x][y]," ")!=0){
            int l = strlen(LR0[x][y]);//当前Ri或Si的长度
 
            if(LR0[x][y][0]=='a'){//acc
                printf("acc        \n");//ACTION与GOTO
                return ;
            }
            else if(LR0[x][y][0]=='S'){//Si
                printf("%-10s \n",LR0[x][y]);//ACTION与GOTO
                int t = calculate(l,LR0[x][y]);//整数
                Status.push(t);
                symbol_ind++;
                symbol_arr[symbol_ind] = str[LR];
                Symbol.push(str[LR]);
                if(t<10){
                    status_ind++;
                    status_arr[status_ind] = LR0[x][y][1];
                }
                else{
                    int k = 1;
                    status_ind++;
                    status_arr[status_ind] = '(';
                    while(k<l){
                        status_ind++;
                        status_arr[status_ind] = LR0[x][y][k];
                        k++;
                    }
                    status_ind++;
                    status_arr[status_ind] = ')';
                }
                LR++;
            }
            else if(LR0[x][y][0]=='r'){//ri,退栈，ACTION和GOTO
                printf("%-10s",LR0[x][y]);
                int t = calculate(l,LR0[x][y]);
                int g = del[t];
                while(g--){
                    Status.pop();
                    Symbol.pop();
                    symbol_arr[symbol_ind] = '\0';
                    symbol_ind--;
                }
                g = del[t];
                while(g>0){
                    if(status_arr[status_ind]==')'){
                        status_arr[status_ind]='\0';
                        status_ind--;
                        for(;;){
                            if(status_arr[status_ind]=='('){
                                status_arr[status_ind]='\0';
                                status_ind--;
                                break;
                            }
                            else{
                                status_arr[status_ind]='\0';
                                status_ind--;
                            }
                        }
                        g--;
                    }
                    else{
                        status_arr[status_ind] = '\0';
                        status_ind--;
                        g--;
                    }
                }
                Symbol.push(head[t]);
                symbol_ind++;
                symbol_arr[symbol_ind] = head[t];
                x = Status.top();
                y = findL(Symbol.top());
                if(x==5&&y==8){
                	t = 10;
                    status_ind++;
                    status_arr[status_ind] = '(';
                    status_ind++;
                    status_arr[status_ind] = '1';
                    status_ind++;
                    status_arr[status_ind] = '0';
                    status_ind++;
                    status_arr[status_ind] = ')';
				}
                else if(x==6&&y==9){
                	t = 11;
                    status_ind++;
                    status_arr[status_ind] = '(';
                    status_ind++;
                    status_arr[status_ind] = '1';
                    status_ind++;
                    status_arr[status_ind] = '1';
                    status_ind++;
                    status_arr[status_ind] = ')';
				}
                else if(x==7&&y==9){
					t=12;
					status_ind++;
                    status_arr[status_ind] = '(';
                    status_ind++;
                    status_arr[status_ind] = '1';
                    status_ind++;
                    status_arr[status_ind] = '2';
                    status_ind++;
                    status_arr[status_ind] = ')';
				}
                else if(x==8&&y==10){
					t=13;status_ind++;
                    status_arr[status_ind] = '(';
                    status_ind++;
                    status_arr[status_ind] = '1';
                    status_ind++;
                    status_arr[status_ind] = '3';
                    status_ind++;
                    status_arr[status_ind] = ')';
				}
                else if(x==9&&y==10){
					t=14;status_ind++;
                    status_arr[status_ind] = '(';
                    status_ind++;
                    status_arr[status_ind] = '1';
                    status_ind++;
                    status_arr[status_ind] = '1';
                    status_ind++;
                    status_arr[status_ind] = '4';
				}
                else{
					t = LR0[x][y][0]-'0';
					status_ind++;
                	status_arr[status_ind] = LR0[x][y][0];
				}
               
                Status.push(t);
                printf("%-10d\n",t);
            }
            else{
                int t = LR0[x][y][0]-'0';
                char ch = ' ';
                printf("%-10c%-10d\n",ch,t);
                Status.push(t);
                status_ind++;
                status_arr[status_ind] = LR0[x][y][0];
                symbol_ind++;
                symbol_arr[symbol_ind] = 'E';
                LR++;
            }
        }
        else{
            error(x,y);
            return ;
        }
    }
   
 
}
void show_table(){
    printf("\n==============================LR(0) Analysis Table============================\n\n");
    printf("-\t+\t-\t*\t/\ti\t(\t)\t#\tE\tT\tF\n");
    for(int i = 0; i <= 15; i++){
        printf("%d",i);
        for(int j = 0 ; j <= 11; j++)
            printf("\t%s",LR0[i][j]);
            cout<<endl;
    }
    cout<<endl;
}

int LR0_main(string s){
    show_table();
    Status.push(0);
    Symbol.push('#');
    char str[200];//输入串
    int i=0;
    for(;i<s.length();++i)
        str[i]=s[i];
    str[i]='#';
    analyze(str,i+1);
    return 0;
}


