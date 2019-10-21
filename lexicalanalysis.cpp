#include <bits/stdc++.h>
using namespace std;
bool ishaverror=false;//�ʷ��Ƿ�����
string  keyword[33]={"program", "var",  "procedure", "begin", "end",  "if", "then", "else", "while", "do", "call", "integer",
 "real", "+", "-", "*", "/", "!", "&&", "||", "<", "<=", ">", ">=", "=", "<>", ":=", ";", ".", ",", "(", ")", ":"};

class eryuanshi//��Ԫʽƥ���ʽ
{
public:
    int clas, valu, id;//clasΪ���valuΪֵ, idΪ�ڼ���
    string name;
};
map<string, eryuanshi> tokens;//�洢��Ԫ����ֹ��ʶ���ڼ������ֵȳ���

void pushkwintomap()//�������ֺ���������ȷ����Ԫ��
{
    eryuanshi ey;
    for(int i=0; i<33; i++){
        ey.clas=i+3; ey.valu=0, ey.name=keyword[i];
        tokens[ey.name]=ey;
    }
}
eryuanshi mytwo;///����������жϵĶ�Ԫʽ����
int whatchar(char ch)//�ж��ַ�����
{
    if('a'<=ch && ch<='z') return 1;//1��ʾ��ĸ
    if('0'<=ch && ch<='9') return 2;//2��ʾ����
    else return 3;//3��ʾ�����ַ�
}
bool judgestring(string str)//�ж����������͵ı����ַ�
{
    for(int i=0; i<33; i++)
        if(str==keyword[i]) return true;
    return false;
}
vector<string> text;//��ȡ����
queue<eryuanshi> token;//�����˳���
list<eryuanshi> gramsent;//�ʷ�˳��
list<eryuanshi>::iterator gramsent_it;
eryuanshi ey;//��Ԫʽ�������ʽ
string str;
int cntid=1, i, j;//��ʶ���ĸ���
bool ishaveletter;//�ж����ֿ�ͷ�Ƿ�����ĸ,������ĸ��Ϊ����
void scan(int len)//ɨ����ɨ�赥�ʲ�ȡ��
{
    while(1){
        if(j<len){
            if( whatchar(text[i][j])==1 ){
                str+=text[i][j];//���ַ�����ĸ
                ishaveletter=true;
            }
            else if( whatchar(text[i][j])==2 ) str+=text[i][j];//���ַ�������
            else{//��������ĸ�����ֽ��������ɸõ���
                j--;//����һ���ַ����´�ȡ��ʱ�������ַ���ʼȡ
                break;//�˳�ѭ��
            }
        }else break;//�������г��ȣ��˳����ɵ���
        j++;//ȡ��һ���ַ�
    }
}
//�洢�����Ϣ��ssΪ�ַ�����idΪ�Ǻ����ַ���
//1��ʾ�����ֺ�������ȣ�2��ʾ���֣� 3��ʾ��ʶ���ͱ���
void pushtoken(string ss, int id)
{
    if(ss=="error"){
        ey.name=ss, ey.id=i+1;
        token.push(ey);//�����������
        return;
    }
    if(id==1){//�����ֺ�����������������
        ey.name=ss, ey.clas=tokens[ss].clas, ey.valu=tokens[ss].valu, ey.id=i+1;
        token.push(ey);
        return;
    }
    if(tokens.find(ss)==tokens.end()){//��Ԫ���Ҳ����ñ�ʶ��,�����ֺ���������ڸ÷�Χ��
        if(id==2){//�洢����
            ey.name=ss, ey.id=i+1;
            stringstream ss;
            ss << str;
            ss >> ey.valu;
            ey.clas=2;
        }else{//�洢��ʶ��
            ey.name=ss, ey.clas=1, ey.valu=cntid++, ey.id=i+1;
        }
        token.push(ey);//�����������
        tokens[ey.name]=ey;//�����Ԫ��
    }else{//��Ԫ���Ѿ����֣�ֱ���ù�����
        ey.name=ss, ey.clas=tokens[ss].clas, ey.valu=tokens[ss].valu, ey.id=i+1;
        token.push(ey);
    }
}
void lexical_analysis()
{
    bool ann=false;//��ע�ͱ�ǣ����������һ��(��/*)Ϊtrue���������*/ʱΪfalse����������
    ifstream ifstr("test.txt");
    text.clear(), tokens.clear();
    pushkwintomap();//��ȡ�����ֵȵĶ�Ԫʽ
    while( !token.empty() ) token.pop();//���ԭ�е��������
    while( getline(ifstr, str) ) text.push_back(str);//���ļ���ȡ����

    str="";//���ڻ�ȡ��������
    for(i=0; i<(int)text.size(); i++){
        int len=(int)text.at(i).length();
        for(j=0; j<len; j++){
            if(ann){//����ǰ���ǿ�ע����
                if(text[i][j-1]=='*' && text[i][j]=='/'){//�Է�j���м��
                   ann=false;
                   continue;
                }
                else continue;
            }

            switch ( whatchar(text[i][j]) ){
                case 1:{//�жϱ�ʶ���ͱ�����
                    str="";//����ǰ���
                    str+=text[i][j++];//�������ĸ�ȷŽ����ȴ���ɵ��ʲ�ȡ��һ���ַ�
                    scan(len);//ɨ�赥��
                    if( judgestring(str) ) pushtoken(str, 1);//�Ǳ�����
                    else{//�Ǳ�����
                        if(str.length()<=8) pushtoken(str, 3);
                        else pushtoken("error", 0);
                    }
                    break;
                }
                case 2:{//�ж�����
                    ishaveletter=false;
                    str="";//����ǰ���
                    str+=text[i][j++];//����������ȷŽ����ȴ���ɵ��ʲ�ȡ��һ���ַ�
                    scan(len);//ɨ�赥��
                    if(ishaveletter) pushtoken("error", 0);//�����д���ĸ������
                    else pushtoken(str, 2);
                    break;
                }
                case 3:{
                    if(text[i][j]==' ' || text[i][j]=='\t') continue;//����ǿո����������
                    string jstr="";//��ǰ�ж��ַ���
                    str="";//����ǰ���
                    str+=text[i][j];//�Ƚ��ַ�����
                    jstr+=text[i][j];//��ǰ�ж��ַ���
                    if(j+1<len){//��������ǰ�г���
                        if( whatchar(text[i][j+1])==3 && text[i][j+1]!=' '){ //����Ƿ���Ҫ��ǰ�ж�
                            jstr+=text[i][j+1];//��ǰ
                            if(jstr=="//"){ j=len; break; }//������ע��
                            if(jstr=="/*"){ j++; ann=true; break; }//������ע��,j++Ϊ������ǰ
                            if(jstr=="*/"){
                                if(!ann){//֮ǰû�г��ֹ���ע�͵ı��
                                    pushtoken("error", 0);
                                    j++;//j++Ϊ������ǰ
                                }
                                break;//�˳����ж���һ������
                            }
                            if(judgestring(jstr)) { j++; pushtoken(jstr, 1); }//j++Ϊ������ǰ
                            else{//�жϵ���������ǲ�����ȷ
                                if(judgestring(str)) pushtoken(str, 1);
                                else pushtoken("error", 0);
                            }
                        }else{//����ǰ
                            if(judgestring(str)) pushtoken(str, 1);
                            else pushtoken("error", 0);
                        }
                    }else{//����Ǿ�ĩ��ֱ�ӽ����ж��Ƿ�Ϊ�����������
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
