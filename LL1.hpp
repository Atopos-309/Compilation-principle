#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

using namespace std;

// Ԥ�������
vector<vector<string> > table(5, vector<string>(9));
// �ķ��Ĳ���ʽ
vector<string> G = {"E->TA", "A->+TA", "A->-TA", "A->e", "T->FB", "B->*FB", "B->/FB", "B->e", "F->i", "F->(E)"};
// �ķ����ŵ��±�ת��
map<char, int> index = {{'E', 0}, {'A', 1}, {'T', 2}, {'B', 3}, {'F', 4}, {'i', 0},  {'+', 1}, {'-', 2}, {'*', 3}, {'/', 4}, {'(', 5}, {')', 6}, {'$', 7}, {'e', 8}};
// �ս��
string terminal("i+-*/()$");
// ���ս��
string nonTerminal("EATBF");
// ����ʽ�Ҳ���first��
vector<string> First = {"i(", "+", "-", "e", "i(", "*", "/", "e", "i", "("};
// ���ս����follow��
vector<string> Follow = {"$)", "$)", "+-$)", "+-$)", "*/+-$)"};



// Ԥ���������
int analysis(string s){
    cout<<endl<<"============================Parsing syntax using LL(1)============================"<<endl;
    int wid = s.length() + 1;
    s.push_back('$');

    //����ջ
    vector<char> analyStack;
    analyStack.push_back('$');
    analyStack.push_back('E');

    // ջ���͵�ǰ����
    char top = '\0', cur = '\0';
    auto ip = s.begin();

    // ���ͷ
    cout << left << setw(wid + 10) << "Stack" << right << setw(wid) << "Input" << "    " << "Output" << endl;
    do{
        // �����ǰջ�͵�ǰʣ������
        string str1(analyStack.begin(), analyStack.end());
        string str2(ip, s.end());
        cout << left << setw(wid + 10) << str1 << right << setw(wid) << str2 << "    ";

        // ջ���͵�ǰ�������
        top = analyStack.back();
        cur = *ip;

        // ջ�����ս���Ż�$
        if (terminal.find(top) != terminal.npos || top == '$'){
            if (top == cur){
                analyStack.pop_back();
                ++ip;
                cout << endl;
            }
            else{
                cout << "����! ��ƥ��,����" << top << endl;
                analyStack.pop_back();
            }
        }
        // ջ�����ս��
        else{
            //����ת��
            int x = index.at(top);
            int y;
            try{
                y = index.at(cur);
            }
            catch (out_of_range){
                cout << "�����ַ��Ƿ�! " << endl;
                break;
            }

            // ����ʽ
            string production = table.at(x).at(y);
            // ����ʽ�ǿ�
            if (!production.empty()){
                if (production == "synch"){    //ͬ��
                    cout << "����!synch,����" << top << endl;
                    analyStack.pop_back();
                }
                else{    //��������
                    analyStack.pop_back();
                    string expr(production.begin() + 3, production.end());
                    if (expr == "e")    //epsilon����ʽ
                        expr = "";
                    // ����ѹջ
                    for (auto iter = expr.rbegin(); iter != expr.rend(); ++iter)
                        analyStack.push_back(*iter);
                    // �������ʽ
                    cout << production << endl;
                }
            }
            else {   //����հ�
                cout << "����! �հ�,����" << *ip << endl;
                ++ip;
            }
        }
    }while (top != '$');
    return 0;
}

int LL1_main(string s){
    // ����Ԥ�������
    // ����G��ÿ������ʽ
    for (auto itG = G.begin(), itFirst = First.begin(); itG != G.end() && itFirst != First.end(); ++itG, ++itFirst){
        // ���ս���±�ת��
        int x = index.at(*(itG->begin()));
        for (auto first = itFirst->begin(); first != itFirst->end(); ++first){
            //����Ҳ��ĵ�һ���ַ��Ƿ��ս������������First��������Ԥ�������
            if (*first != 'e'){
                int y = index.at(*first);
                table.at(x).at(y) = *itG;
            }
            //����Ҳ��ĵ�һ���ַ��ǿմ��������󲿵�Follow��������Ԥ�������
            else{
                for (auto follow = Follow.at(x).begin(); follow != Follow.at(x).end(); ++follow){
                    int y = index.at(*follow);
                    table.at(x).at(y) = *itG;
                }
            }
        }
    }
    // д��ͬ����Ϣ
    for (string::size_type i = 0; i < nonTerminal.length(); ++i){
        int x = index.at(nonTerminal.at(i));
        for (vector<string>::size_type j = 0; j < Follow.at(i).length(); ++j){
            int y = index.at(Follow.at(i).at(j));
            if (table.at(x).at(y).empty())
                table[x][y] = "synch";
        }
    }
    // ���Ԥ�������
    cout<<endl<<"==============================LR(0) Analysis Table============================"<<endl<<endl;
    // ����ս��
    for (string::size_type i = 0; i < terminal.size(); ++i)
        cout << '\t' << terminal[i];
    cout << endl;
    // ������ս��
    for (string::size_type x = 0; x < nonTerminal.size(); ++x){
        cout << nonTerminal[x];
        // �������ʽ
        for (string::size_type y = 0; y < table.at(x).size(); ++y)
            cout << '\t' << table.at(x).at(y);
        cout << endl;
    }
    cout << endl;
    return analysis(s);
}


