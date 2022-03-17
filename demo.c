/*demo.c by 19335206 Wei Yuanxi*/
int main(){
    int test_a=1,test_b=223;
    float f=1.234;
    if(test_a<=test_b){
        printf("yes");
    }
    char c[5]='a';
    //testing
    for(int i=0;i<5;++i){
        test_a=test_a>>1;
        test_b+=test_a;
        if(i==3)
            break;
    }
}
#