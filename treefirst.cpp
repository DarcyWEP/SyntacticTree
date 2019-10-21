#include <bits/stdc++.h>
#include "state.cpp"
using namespace std;

tree_node root;//根节点s
void creatroot()//创建根节点 <程序> -> program id;<程序体>
{
    //先检查尾部
    if(gramsent.back().name!=".") out_error(14, "");
    else{
        eryuanshi erys=gramsent.back();
        gramsent.pop_back();
        if(gramsent.back().name!="end") out_error(14, "");
        else gramsent.push_back(erys);
    }
    root.name="<程序>";
    root.children.clear();

    if(!gramsent.empty() && gramsent.front().name=="program") root.pushtree_node("program");//<程序> - program
    else out_error(0, "程序定义符(program)");//报错

    if(!istreerror){// <程序> - id
        if(gramsent.front().clas==1 && gramsent.front().valu==1 && !gramsent.empty()){
            tree.name="<标识符>"; tree.children.clear();
            tree.pushtree_node(gramsent.front().name);
            root.children.push_back(tree); //检查程序标识符id
        }
        else out_error(0, "程序标识符");//报错
    }

    if(!istreerror){//<程序> - ;
        if(gramsent.front().name==";" && !gramsent.empty()) root.pushtree_node(";");// 检查分号";"
        else out_error(0, "(;)");//报错
    }
}
tree_node programbody;//定义<程序体>
tree_node vardecl;//定义<变量说明>
vector<tree_node> vardecl_table;//定义<变量说明表>
vector<tree_node> var_table;//定义<变量表>
int vardecl_table_num=-1;
int var_table_num=-1;

tree_node var;//定义<变量>
tree_node type;//定义<类型>

void creatvar_table();
void creatvardecl_table();//变量说明表
void creatvardecl();//变量说明

void creatprogrambody() //<程序体> -> <变量说明>; begin<语句串>end.
{
    var_table.clear(), vardecl_table.clear();
    programbody.children.clear();
    programbody.name="<程序体>";

    vardecl.children.clear(); //<变量说明> -> var<变量说明表>;
    vardecl.name="<变量说明>";

    //分程序第一步，判断是否有变量说明

    if(!istreerror){// <程序体> - <变量说明>
        if(!gramsent.empty() && gramsent.front().name=="var"){//变量说明第一个是不是var
            vardecl.pushtree_node("var");///<变量说明> - var
            creatvardecl();
            programbody.children.push_back(vardecl);
        }
        else out_error(0, "变量声明");//报错
    }

    if(!istreerror){// <程序体> - begin
        if(gramsent.front().name=="begin" && !gramsent.empty()) programbody.pushtree_node("begin");
        else out_error(0, "程序开始标志(begin)");//报错
    }

    //cout << gramsent.back().name << endl;
    out_state_string();//<程序体> - <语句串>
    if(!istreerror) programbody.children.push_back(statement.at(0));
    //for(int i=0; i<statestring.children.size(); i++) cout << statestring.children.at(i).name << " ";

    if(!istreerror ){// <程序体> - end.
        if(!gramsent.empty()){
            gramsent_it=gramsent.end(); *gramsent_it--, *gramsent_it--;//取end
            if(gramsent.back().name=="." && gramsent_it->name=="end"){
                tree_node tr;
                ///存入end
                tr.name=gramsent_it->name;
                tr.trey=*gramsent_it;
                tr.children.clear();
                programbody.children.push_back(tr);
                ///存入.
                tr.name=gramsent.back().name;
                tr.trey=gramsent.back();
                tr.children.clear();
                programbody.children.push_back(tr);
                gramsent.pop_back();
                gramsent.pop_back();
            }else out_error(14, "");//报错
        }else cout << "程序不完整！！！" << endl;
    }

    root.children.push_back(programbody);//<程序> - <程序体>
}

void creatvardecl()//<变量说明> - <变量说明表> ;
{
    if(!istreerror){
        if(!gramsent.empty() && gramsent.front().clas==1){
            vardecl_table_num++; vardecl_table.push_back(tree);
            vardecl_table.at(vardecl_table_num).children.clear();
            vardecl_table.at(vardecl_table_num).name="<变量说明表>";
            creatvardecl_table();///<变量说明> - <变量说明表>
            if(!istreerror){
                gramsent_it=gramsent.begin(), *gramsent_it++;
                if(!gramsent.empty() && gramsent_it->name=="begin"){
                    for(int i=(int)vardecl_table.size()-2; i>=0; i--){
                        vardecl_table.at(i).children.push_back(vardecl_table.at(i+1));
                    }
                    vardecl.children.push_back(vardecl_table.at(0));///<变量说明> - <变量说明表>
                    if(gramsent.front().name==";") vardecl.pushtree_node(";");///<变量说明> - ;
                    else out_error(0, "(;)");
                }else if(!gramsent.empty() && gramsent_it->clas==1){
                    vardecl_table.at(vardecl_table_num).pushtree_node(";");///<变量说明表> -> <变量表>:<类型> ;
                    creatvardecl();
                }//else out_error(12, "");
            }
        }else out_error(0, "变量说明表");
    }
}

void creatvardecl_table()//<变量说明表> -> <变量表>:<类型> | <变量表>:<类型> ; <变量说明表>
{
    if(!istreerror){
        if(!gramsent.empty()){
            if(gramsent.front().clas==1){///<变量说明表> -> <变量表>
                var_table_num++, var_table.push_back(tree);
                var_table.at(var_table_num).children.clear();
                var_table.at(var_table_num).name="<变量表>";
                creatvar_table();//创建这个变量表
                creatvardecl_table();//不管接下来的是不是变量，继续运行这个函数
            }else if(gramsent.front().name==":"){///<变量说明表> -> :
                for(int i=(int)var_table.size()-2; i>=0; i--){ ///整合变量表
                    var_table.at(i).children.push_back(var_table.at(i+1));
                }
                vardecl_table.at(vardecl_table_num).children.push_back(var_table.at(0));
                var_table.clear(), var_table_num=-1;
                vardecl_table.at(vardecl_table_num).pushtree_node(":");
                if(!istreerror){///<变量说明表> -> <类型>
                    if(!gramsent.empty() && gramsent.front().name=="integer"){
                        type.children.clear(), type.pushtree_node("integer");
                        type.name="<类型>";
                        vardecl_table.at(vardecl_table_num).children.push_back(type);
                    }else out_error(0, "(integer)");
                }
            }else out_error(12, "");
        }
    }
}

void creatvar_table()//id为1表示是变量，2表示是符号 <变量表> -> <变量> | <变量>, <变量表>
{
    if(!istreerror){///<变量表> - <变量>
        if(gramsent.front().clas==1 && !gramsent.empty()){
            var.name="<变量>";
            var.children.clear();
            varmap[ gramsent.front().name ]=true;
            var.pushtree_node(gramsent.front().name);
            var_table.at(var_table_num).children.push_back(var);
        }else out_error(12, "");//报错
    }
    if(!istreerror){///<变量表> - ,
        if(!gramsent.empty() && gramsent.front().name=="," ){
            var_table.at(var_table_num).pushtree_node(gramsent.front().name);//变量分割符
            if(gramsent.empty() || gramsent.front().clas!=1) out_error(12, "");
        }
    }
}

void tree_first()
{
    creatroot();
    if(!istreerror) creatprogrambody();
}
