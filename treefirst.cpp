#include <bits/stdc++.h>
#include "state.cpp"
using namespace std;

tree_node root;//���ڵ�s
void creatroot()//�������ڵ� <����> -> program id;<������>
{
    //�ȼ��β��
    if(gramsent.back().name!=".") out_error(14, "");
    else{
        eryuanshi erys=gramsent.back();
        gramsent.pop_back();
        if(gramsent.back().name!="end") out_error(14, "");
        else gramsent.push_back(erys);
    }
    root.name="<����>";
    root.children.clear();

    if(!gramsent.empty() && gramsent.front().name=="program") root.pushtree_node("program");//<����> - program
    else out_error(0, "�������(program)");//����

    if(!istreerror){// <����> - id
        if(gramsent.front().clas==1 && gramsent.front().valu==1 && !gramsent.empty()){
            tree.name="<��ʶ��>"; tree.children.clear();
            tree.pushtree_node(gramsent.front().name);
            root.children.push_back(tree); //�������ʶ��id
        }
        else out_error(0, "�����ʶ��");//����
    }

    if(!istreerror){//<����> - ;
        if(gramsent.front().name==";" && !gramsent.empty()) root.pushtree_node(";");// ���ֺ�";"
        else out_error(0, "(;)");//����
    }
}
tree_node programbody;//����<������>
tree_node vardecl;//����<����˵��>
vector<tree_node> vardecl_table;//����<����˵����>
vector<tree_node> var_table;//����<������>
int vardecl_table_num=-1;
int var_table_num=-1;

tree_node var;//����<����>
tree_node type;//����<����>

void creatvar_table();
void creatvardecl_table();//����˵����
void creatvardecl();//����˵��

void creatprogrambody() //<������> -> <����˵��>; begin<��䴮>end.
{
    var_table.clear(), vardecl_table.clear();
    programbody.children.clear();
    programbody.name="<������>";

    vardecl.children.clear(); //<����˵��> -> var<����˵����>;
    vardecl.name="<����˵��>";

    //�ֳ����һ�����ж��Ƿ��б���˵��

    if(!istreerror){// <������> - <����˵��>
        if(!gramsent.empty() && gramsent.front().name=="var"){//����˵����һ���ǲ���var
            vardecl.pushtree_node("var");///<����˵��> - var
            creatvardecl();
            programbody.children.push_back(vardecl);
        }
        else out_error(0, "��������");//����
    }

    if(!istreerror){// <������> - begin
        if(gramsent.front().name=="begin" && !gramsent.empty()) programbody.pushtree_node("begin");
        else out_error(0, "����ʼ��־(begin)");//����
    }

    //cout << gramsent.back().name << endl;
    out_state_string();//<������> - <��䴮>
    if(!istreerror) programbody.children.push_back(statement.at(0));
    //for(int i=0; i<statestring.children.size(); i++) cout << statestring.children.at(i).name << " ";

    if(!istreerror ){// <������> - end.
        if(!gramsent.empty()){
            gramsent_it=gramsent.end(); *gramsent_it--, *gramsent_it--;//ȡend
            if(gramsent.back().name=="." && gramsent_it->name=="end"){
                tree_node tr;
                ///����end
                tr.name=gramsent_it->name;
                tr.trey=*gramsent_it;
                tr.children.clear();
                programbody.children.push_back(tr);
                ///����.
                tr.name=gramsent.back().name;
                tr.trey=gramsent.back();
                tr.children.clear();
                programbody.children.push_back(tr);
                gramsent.pop_back();
                gramsent.pop_back();
            }else out_error(14, "");//����
        }else cout << "��������������" << endl;
    }

    root.children.push_back(programbody);//<����> - <������>
}

void creatvardecl()//<����˵��> - <����˵����> ;
{
    if(!istreerror){
        if(!gramsent.empty() && gramsent.front().clas==1){
            vardecl_table_num++; vardecl_table.push_back(tree);
            vardecl_table.at(vardecl_table_num).children.clear();
            vardecl_table.at(vardecl_table_num).name="<����˵����>";
            creatvardecl_table();///<����˵��> - <����˵����>
            if(!istreerror){
                gramsent_it=gramsent.begin(), *gramsent_it++;
                if(!gramsent.empty() && gramsent_it->name=="begin"){
                    for(int i=(int)vardecl_table.size()-2; i>=0; i--){
                        vardecl_table.at(i).children.push_back(vardecl_table.at(i+1));
                    }
                    vardecl.children.push_back(vardecl_table.at(0));///<����˵��> - <����˵����>
                    if(gramsent.front().name==";") vardecl.pushtree_node(";");///<����˵��> - ;
                    else out_error(0, "(;)");
                }else if(!gramsent.empty() && gramsent_it->clas==1){
                    vardecl_table.at(vardecl_table_num).pushtree_node(";");///<����˵����> -> <������>:<����> ;
                    creatvardecl();
                }//else out_error(12, "");
            }
        }else out_error(0, "����˵����");
    }
}

void creatvardecl_table()//<����˵����> -> <������>:<����> | <������>:<����> ; <����˵����>
{
    if(!istreerror){
        if(!gramsent.empty()){
            if(gramsent.front().clas==1){///<����˵����> -> <������>
                var_table_num++, var_table.push_back(tree);
                var_table.at(var_table_num).children.clear();
                var_table.at(var_table_num).name="<������>";
                creatvar_table();//�������������
                creatvardecl_table();//���ܽ��������ǲ��Ǳ��������������������
            }else if(gramsent.front().name==":"){///<����˵����> -> :
                for(int i=(int)var_table.size()-2; i>=0; i--){ ///���ϱ�����
                    var_table.at(i).children.push_back(var_table.at(i+1));
                }
                vardecl_table.at(vardecl_table_num).children.push_back(var_table.at(0));
                var_table.clear(), var_table_num=-1;
                vardecl_table.at(vardecl_table_num).pushtree_node(":");
                if(!istreerror){///<����˵����> -> <����>
                    if(!gramsent.empty() && gramsent.front().name=="integer"){
                        type.children.clear(), type.pushtree_node("integer");
                        type.name="<����>";
                        vardecl_table.at(vardecl_table_num).children.push_back(type);
                    }else out_error(0, "(integer)");
                }
            }else out_error(12, "");
        }
    }
}

void creatvar_table()//idΪ1��ʾ�Ǳ�����2��ʾ�Ƿ��� <������> -> <����> | <����>, <������>
{
    if(!istreerror){///<������> - <����>
        if(gramsent.front().clas==1 && !gramsent.empty()){
            var.name="<����>";
            var.children.clear();
            varmap[ gramsent.front().name ]=true;
            var.pushtree_node(gramsent.front().name);
            var_table.at(var_table_num).children.push_back(var);
        }else out_error(12, "");//����
    }
    if(!istreerror){///<������> - ,
        if(!gramsent.empty() && gramsent.front().name=="," ){
            var_table.at(var_table_num).pushtree_node(gramsent.front().name);//�����ָ��
            if(gramsent.empty() || gramsent.front().clas!=1) out_error(12, "");
        }
    }
}

void tree_first()
{
    creatroot();
    if(!istreerror) creatprogrambody();
}
