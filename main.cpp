
#include "LL1.hpp"
#include "LR0.hpp"

int main(){
    string s;
    cout<<"Please enter an arithmetic expression:"<<endl;
    cin>>s;
    LL1_main(s);
    LR0_main(s);
}