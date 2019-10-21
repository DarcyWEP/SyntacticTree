#include <bits/stdc++.h>
using namespace std;
bool istreerror=false;
int error_id=1;//出错的行
void out_error(int id, string error)//id为出错的类型（10以后的为特殊错误）， error为可能缺少的东西
{
    istreerror=true;
    if(id==1) cout << "第" << error_id << "行或第" << error_id+1 <<  "行可能缺少标识符，请检查！！！" << endl;
    else if(id==0) cout << "第" << error_id <<  "行可能缺少" << error <<"，请检查！！！" << endl;
    else if(id==2) cout << "第" << error_id <<  "行可能多出" << error <<"，请检查！！！" << endl;
    else if(id==12) cout << "var后的可能出现不可识别的字符，请检查！！！" << endl;
    else if(id==13) cout << "（var）后定义的数据类型错误，请检查！！！" << endl;
    else if(id==14) cout << "缺少程序结束标志(end)或(.)" << endl;
    else if(id==15) cout <<  "第" << error_id << "行存在未定义的变量！！！" << endl;
}
