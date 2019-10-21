#include <bits/stdc++.h>
using namespace std;
bool ishaverror=false;//词法是否有误
string  keyword[33]={"program", "var",  "procedure", "begin", "end",  "if", "then", "else", "while", "do", "call", "integer",
 "real", "+", "-", "*", "/", "!", "&&", "||", "<", "<=", ">", ">=", "=", "<>", ":=", ";", ".", ",", "(", ")", ":"};

class eryuanshi//二元式匹配格式
{
public:
    int clas, valu, id;//clas为类别，valu为值, id为第几行
    string name;
};
map<string, eryuanshi> tokens;//存储二元表，防止标识符第几个数字等出错

void pushkwintomap()//将保留字和运算符等先放入二元表
{
    eryuanshi ey;
    for(int i=0; i<33; i++){
        ey.clas=i+3; ey.valu=0, ey.name=keyword[i];
        tokens[ey.name]=ey;
    }
}
eryuanshi mytwo;///用于在这边判断的二元式返回
int whatchar(char ch)//判断字符类型
{
    if('a'<=ch && ch<='z') return 1;//1表示字母
    if('0'<=ch && ch<='9') return 2;//2表示数字
    else return 3;//3表示其它字符
}
bool judgestring(string str)//判断是那种类型的保留字符
{
    for(int i=0; i<33; i++)
        if(str==keyword[i]) return true;
    return false;
}
vector<string> text;//获取输入
queue<eryuanshi> token;//输出的顺序等
list<eryuanshi> gramsent;//词法顺序
list<eryuanshi>::iterator gramsent_it;
eryuanshi ey;//二元式的输出格式
string str;
int cntid=1, i, j;//标识符的个数
bool ishaveletter;//判断数字开头是否含有字母,含有字母则为出错
void scan(int len)//扫描器扫描单词并取出
{
    while(1){
        if(j<len){
            if( whatchar(text[i][j])==1 ){
                str+=text[i][j];//该字符是字母
                ishaveletter=true;
            }
            else if( whatchar(text[i][j])==2 ) str+=text[i][j];//该字符是数字
            else{//遇到非字母和数字结束，生成该单词
                j--;//回退一个字符，下次取的时候从这个字符开始取
                break;//退出循环
            }
        }else break;//超出本行长度，退出生成单词
        j++;//取下一个字符
    }
}
//存储输出信息，ss为字符本身，id为是何种字符，
//1表示保留字和运算符等，2表示数字， 3表示标识符和变量
void pushtoken(string ss, int id)
{
    if(ss=="error"){
        ey.name=ss, ey.id=i+1;
        token.push(ey);//放入输出队列
        return;
    }
    if(id==1){//保留字和运算符放入输出队列
        ey.name=ss, ey.clas=tokens[ss].clas, ey.valu=tokens[ss].valu, ey.id=i+1;
        token.push(ey);
        return;
    }
    if(tokens.find(ss)==tokens.end()){//二元表找不到该标识符,保留字和运算符不在该范围内
        if(id==2){//存储数字
            ey.name=ss, ey.id=i+1;
            stringstream ss;
            ss << str;
            ss >> ey.valu;
            ey.clas=2;
        }else{//存储标识符
            ey.name=ss, ey.clas=1, ey.valu=cntid++, ey.id=i+1;
        }
        token.push(ey);//放入输出队列
        tokens[ey.name]=ey;//存入二元表
    }else{//二元表已经出现，直接拿过来用
        ey.name=ss, ey.clas=tokens[ss].clas, ey.valu=tokens[ss].valu, ey.id=i+1;
        token.push(ey);
    }
}
void lexical_analysis()
{
    bool ann=false;//块注释标记，如果遇到第一个(即/*)为true，如果遇到*/时为false，则发生错误
    ifstream ifstr("test.txt");
    text.clear(), tokens.clear();
    pushkwintomap();//获取保留字等的二元式
    while( !token.empty() ) token.pop();//清空原有的输出单词
    while( getline(ifstr, str) ) text.push_back(str);//从文件获取输入

    str="";//用于获取单个单词
    for(i=0; i<(int)text.size(); i++){
        int len=(int)text.at(i).length();
        for(j=0; j<len; j++){
            if(ann){//遇到前面是块注释区
                if(text[i][j-1]=='*' && text[i][j]=='/'){//以防j是中间符
                   ann=false;
                   continue;
                }
                else continue;
            }

            switch ( whatchar(text[i][j]) ){
                case 1:{//判断标识符和保留字
                    str="";//启用前清空
                    str+=text[i][j++];//如果是字母先放进，等待组成单词并取下一个字符
                    scan(len);//扫描单词
                    if( judgestring(str) ) pushtoken(str, 1);//是保留字
                    else{//非保留字
                        if(str.length()<=8) pushtoken(str, 3);
                        else pushtoken("error", 0);
                    }
                    break;
                }
                case 2:{//判断数字
                    ishaveletter=false;
                    str="";//启用前清空
                    str+=text[i][j++];//如果是数字先放进，等待组成单词并取下一个字符
                    scan(len);//扫描单词
                    if(ishaveletter) pushtoken("error", 0);//数字中带字母，报错
                    else pushtoken(str, 2);
                    break;
                }
                case 3:{
                    if(text[i][j]==' ' || text[i][j]=='\t') continue;//如果是空格则继续进行
                    string jstr="";//超前判断字符用
                    str="";//启用前清空
                    str+=text[i][j];//先将字符放入
                    jstr+=text[i][j];//超前判断字符用
                    if(j+1<len){//不超出当前行长度
                        if( whatchar(text[i][j+1])==3 && text[i][j+1]!=' '){ //检查是否需要超前判断
                            jstr+=text[i][j+1];//超前
                            if(jstr=="//"){ j=len; break; }//遇到行注释
                            if(jstr=="/*"){ j++; ann=true; break; }//遇到块注释,j++为真正超前
                            if(jstr=="*/"){
                                if(!ann){//之前没有出现过块注释的标记
                                    pushtoken("error", 0);
                                    j++;//j++为真正超前
                                }
                                break;//退出，判断下一个单词
                            }
                            if(judgestring(jstr)) { j++; pushtoken(jstr, 1); }//j++为真正超前
                            else{//判断单个运算符是不是正确
                                if(judgestring(str)) pushtoken(str, 1);
                                else pushtoken("error", 0);
                            }
                        }else{//不超前
                            if(judgestring(str)) pushtoken(str, 1);
                            else pushtoken("error", 0);
                        }
                    }else{//如果是句末，直接进行判断是否为运算符或界符等
                        if(judgestring(str)) pushtoken(str, 1);
                        else pushtoken("error", 0);
                    }
                    break;
                }
            }

        }
    }
    ofstream ofstr;
    ofstr.open(("result.txt"));
    while(!token.empty()){
        if( token.front().name=="error"){
            ofstr << "**error(" << token.front().id << ")**" << endl;
            ishaverror=true;
        }
        else ofstr << "(" << token.front().clas << "," << token.front().valu << ")" << token.front().name << endl;
        gramsent.push_back(token.front());
        token.pop();
    }
    return;
}
