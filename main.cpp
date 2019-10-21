#include <bits/stdc++.h>
#include "outree.cpp"
using namespace std;
int main()
{
    lexical_analysis();
    if(ishaverror) cout << "词法分析出错，具体见result.txt!!!" << endl;//词法分析有误
    else {
        tree_first();
        out_tree();
    }
    return 0;
}
