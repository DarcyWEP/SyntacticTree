#include <bits/stdc++.h>
#include "outree.cpp"
using namespace std;
int main()
{
    lexical_analysis();
    if(ishaverror) cout << "�ʷ��������������result.txt!!!" << endl;//�ʷ���������
    else {
        tree_first();
        out_tree();
    }
    return 0;
}
