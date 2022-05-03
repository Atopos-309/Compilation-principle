#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

using namespace std;

// 预测分析表
vector<vector<string> > table(5, vector<string>(9));
// 文法的产生式
vector<string> G = {"E->TA", "A->+TA", "A->-TA", "A->e", "T->FB", "B->*FB", "B->/FB", "B->e", "F->i", "F->(E)"};
// 文法符号到下标转换
map<char, int> index = {{'E', 0}, {'A', 1}, {'T', 2}, {'B', 3}, {'F', 4}, {'i', 0},  {'+', 1}, {'-', 2}, {'*', 3}, {'/', 4}, {'(', 5}, {')', 6}, {'$', 7}, {'e', 8}};
// 终结符
string terminal("i+-*/()$");
// 非终结符
string nonTerminal("EATBF");
// 产生式右部的first集
vector<string> First = {"i(", "+", "-", "e", "i(", "*", "/", "e", "i", "("};
// 非终结符的follow集
vector<string> Follow = {"$)", "$)", "+-$)", "+-$)", "*/+-$)"};



// 预测分析过程
int analysis(string s){
    cout<<endl<<"============================Parsing syntax using LL(1)============================"<<endl;
    int wid = s.length() + 1;
    s.push_back('$');

    //分析栈
    vector<char> analyStack;
    analyStack.push_back('$');
    analyStack.push_back('E');

    // 栈顶和当前输入
    char top = '\0', cur = '\0';
    auto ip = s.begin();

    // 输出头
    cout << left << setw(wid + 10) << "Stack" << right << setw(wid) << "Input" << "    " << "Output" << endl;
    do{
        // 输出当前栈和当前剩余输入
        string str1(analyStack.begin(), analyStack.end());
        string str2(ip, s.end());
        cout << left << setw(wid + 10) << str1 << right << setw(wid) << str2 << "    ";

        // 栈顶和当前输入符号
        top = analyStack.back();
        cur = *ip;

        // 栈顶是终结符号或$
        if (terminal.find(top) != terminal.npos || top == '$'){
            if (top == cur){
                analyStack.pop_back();
                ++ip;
                cout << endl;
            }
            else{
                cout << "出错! 不匹配,弹出" << top << endl;
                analyStack.pop_back();
            }
        }
        // 栈顶非终结符
        else{
            //坐标转换
            int x = index.at(top);
            int y;
            try{
                y = index.at(cur);
            }
            catch (out_of_range){
                cout << "输入字符非法! " << endl;
                break;
            }

            // 产生式
            string production = table.at(x).at(y);
            // 产生式非空
            if (!production.empty()){
                if (production == "synch"){    //同步
                    cout << "出错!synch,弹出" << top << endl;
                    analyStack.pop_back();
                }
                else{    //正常分析
                    analyStack.pop_back();
                    string expr(production.begin() + 3, production.end());
                    if (expr == "e")    //epsilon产生式
                        expr = "";
                    // 逆序压栈
                    for (auto iter = expr.rbegin(); iter != expr.rend(); ++iter)
                        analyStack.push_back(*iter);
                    // 输出产生式
                    cout << production << endl;
                }
            }
            else {   //表项空白
                cout << "出错! 空白,跳过" << *ip << endl;
                ++ip;
            }
        }
    }while (top != '$');
    return 0;
}

int LL1_main(string s){
    // 构造预测分析表
    // 遍历G的每个产生式
    for (auto itG = G.begin(), itFirst = First.begin(); itG != G.end() && itFirst != First.end(); ++itG, ++itFirst){
        // 非终结符下标转换
        int x = index.at(*(itG->begin()));
        for (auto first = itFirst->begin(); first != itFirst->end(); ++first){
            //如果右部的第一个字符是非终结符，遍历它的First集，更新预测分析表
            if (*first != 'e'){
                int y = index.at(*first);
                table.at(x).at(y) = *itG;
            }
            //如果右部的第一个字符是空串，遍历左部的Follow集，更新预测分析表
            else{
                for (auto follow = Follow.at(x).begin(); follow != Follow.at(x).end(); ++follow){
                    int y = index.at(*follow);
                    table.at(x).at(y) = *itG;
                }
            }
        }
    }
    // 写入同步信息
    for (string::size_type i = 0; i < nonTerminal.length(); ++i){
        int x = index.at(nonTerminal.at(i));
        for (vector<string>::size_type j = 0; j < Follow.at(i).length(); ++j){
            int y = index.at(Follow.at(i).at(j));
            if (table.at(x).at(y).empty())
                table[x][y] = "synch";
        }
    }
    // 输出预测分析表
    cout<<endl<<"==============================LR(0) Analysis Table============================"<<endl<<endl;
    // 输出终结符
    for (string::size_type i = 0; i < terminal.size(); ++i)
        cout << '\t' << terminal[i];
    cout << endl;
    // 输出非终结符
    for (string::size_type x = 0; x < nonTerminal.size(); ++x){
        cout << nonTerminal[x];
        // 输出产生式
        for (string::size_type y = 0; y < table.at(x).size(); ++y)
            cout << '\t' << table.at(x).at(y);
        cout << endl;
    }
    cout << endl;
    return analysis(s);
}


