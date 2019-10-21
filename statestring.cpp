#include <bits/stdc++.h>
#include "lexicalanalysis.cpp"
#include "outerror.cpp"
using namespace std;
map<string, bool> varmap;//定义的变量符
bool is_have_this_var(string na)
{
    return varmap.find(na)!=varmap.end();
}
class tree_node
{
    public:
        string name;
        eryuanshi trey;
        vector<tree_node> children;
    public:
        void pushtree_node(string na)
        {
            tree_node tr;//创建root的子节点用
            tr.name=na;
            tr.trey=gramsent.front();
            tr.children.clear();
            children.push_back(tr);//初始化并存入
            gramsent.pop_front();//如果是弹出
            error_id=tr.trey.id;
        }
        void push_end_state()
        {
            gramsent_it=gramsent.begin(); *gramsent_it++;
            if(gramsent_it->name=="end" && gramsent.front().name!="end") out_error(2, "(;)");
            else if(gramsent.front().name==";") pushtree_node(";");
        }
};
bool check_end_state()
{
    if(!gramsent.empty() && (gramsent.front().name==";" || gramsent.front().name=="end") ) return true;
    else{
        out_error(0, "句尾标志(end)或(;)");
        return false;
    }
}
tree_node tree;

tree_node statestring;//<语句串> -> <语句>|<语句>;<语句串>
vector<tree_node> statement;//<语句> -> <复合语句>|<赋值语句>|<条件语句>|<循环语句>
vector<tree_node> comstate;//< 复合语句> -> begin <语句串>end
tree_node asstate;//<赋值语句> -> id:=<表达式>
vector<tree_node> constate;//<条件语句> -> if<布尔表达式>　then　<语句>｜if<布尔表达式>　then　<语句> 　else <语句>
vector<tree_node> loopstate;//<循环语句> -> while　<布尔表达式>　do　<语句>
vector<tree_node> express;// <算术表达式> -> <项>｜<项>+<算术表达式> | <项>-<算术表达式>
vector<tree_node> nape;//<项> -> <因子>｜<因子>*<项> | <因子>/<项>
vector<tree_node> factor;//<因子> -> (<算术表达式>) | <变量> | <常数>
vector<tree_node> bool_express;//<布尔表达式> -> <关系表达式> | ！<布尔表达式> | <布尔表达式> && <布尔表达式>  | <布尔表达式> ||　<布尔表达式>
vector<tree_node> relation_express;//< 关系表达式> -> <算术表达式><关系><算术表达式>
int is_have_left=0;
void state_initial()
{
    statement.clear();
    nape.clear();
    factor.clear();
    bool_express.clear();
    relation_express.clear();
    statestring.name="<语句串>";
    statestring.children.clear();
    comstate.clear();
    constate.clear();
    loopstate.clear();
    asstate.name="<赋值语句>";
    asstate.children.clear();
    express.clear();
}

void state_ment(string na_state);//<语句串> -> <语句> | <语句> ; <语句串>
void bool_state_ment(string na_state);
void state_string(string na_state);//<语句> -> <复合语句>|<赋值语句>|<条件语句>|<循环语句>
void ass_tate();//<赋值语句> -> id:=<表达式>
void expression();// <表达式> -> (<表达式>)｜<因子>+<因子> | <因子>-<因子> | <因子>*<因子> | <因子>/<因子>
void bool_expression();//关系表达式中的表达式
void creatboolexpress();
void con_state();//<条件语句> -> if<布尔表达式>　then　<语句>｜if<布尔表达式>　then　<语句> 　else <语句>
void com_state();//< 复合语句> -> begin <语句串>end
void loop_state();//<循环语句> -> while　<布尔表达式>　do　<语句>
void creatnape();//创建项
void creatfactor();//创建因子
void relationexpress();//创建关系表达式


int state_ment_num=-1;
int factor_num=-1;
int nape_num=-1;
int bool_express_num=-1;
int relation_express_num=-1;
int con_state_num=-1;//切套语句
int com_state_num=-1;
int loop_state_num=-1;
int express_num=-1;




void state_string(string na_state)//<语句> -> <复合语句>|<赋值语句>|<条件语句>|<循环语句>
{
    tree_node tr;
    if(!istreerror){
        if(!gramsent.empty()){
            if(gramsent.front().clas==1){//<语句> - <赋值语句>
                asstate.name="<赋值语句>", asstate.children.clear();
                statestring.name="<语句>", statestring.children.clear();
                ass_tate();
                if(!istreerror) statement.at(state_ment_num).children.push_back(asstate);///<语句> -> <赋值语句>

            }else if(gramsent.front().name=="begin"){//<语句> - <复合语句>
                if(na_state=="statestring") comstate.clear();//第一个复合语句，须将前面的清空
                com_state_num++;//记录是切套的第几个复合语句
                comstate.push_back(tr);
                comstate.at(com_state_num).name="<复合语句>";
                comstate.at(com_state_num).children.clear();
                com_state();
                if(!istreerror){
                    statement.at(state_ment_num).children.push_back(comstate.at(com_state_num));//<语句> -> <赋值语句>
                    com_state_num--; comstate.pop_back();
                }

            }else if(gramsent.front().name=="if"){//<语句> - <条件语句>
                if(na_state=="statestring") constate.clear();//第一个条件语句，须将前面的清空
                con_state_num++;//记录是切套的第几个条件语句
                constate.push_back(tr);
                constate.at(con_state_num).name="<条件语句>";
                constate.at(con_state_num).children.clear();
                con_state();

                if(!istreerror){
                    statement.at(state_ment_num).children.push_back(constate.at(con_state_num));//<语句串> -> <条件语句>
                    con_state_num--; constate.pop_back();
                }

            }else if(gramsent.front().name=="while"){//<语句串> - <循环语句>
                if(na_state=="statestring") loopstate.clear();//第一个条件语句，须将前面的清空
                loop_state_num++;//记录是切套的第几个条件语句
                loopstate.push_back(tr);
                loopstate.at(loop_state_num).name="<循环语句>";
                loopstate.at(loop_state_num).children.clear();
                loop_state();

                if(!istreerror){
                    statement.at(state_ment_num).children.push_back(loopstate.at(loop_state_num));//<语句串> -> <循环语句>
                    loop_state_num--; loopstate.pop_back();
                }
            }
        }
    }
}

void bool_state_ment(string na_state)
{
    if(!istreerror){//<语句串> -> <语句>|<语句>;<语句串>
        if(!gramsent.empty() && ( gramsent.front().clas==1 || gramsent.front().name=="begin"
                || gramsent.front().name=="if" || gramsent.front().name=="while" )){//如果是语句，则进行语句判断
            int id=state_ment_num;
            state_ment_num++, statement.push_back(tree);
            statement.at(state_ment_num).children.clear();
            statement.at(state_ment_num).name="<语句>";
            state_string(na_state);
            if(!istreerror){
                if(na_state=="loopstate") loopstate.at(loop_state_num).children.push_back(statement.at(id+1));
                else constate.at(con_state_num).children.push_back(statement.at(id+1));
                statement.pop_back(), state_ment_num--;
            }
        }else out_error(2, " ; ");
    }
}

void state_ment(string na_state)
{
    if(!istreerror){//<语句串> -> <语句>|<语句>;<语句串>
        if(!gramsent.empty() && ( gramsent.front().clas==1 || gramsent.front().name=="begin"
                || gramsent.front().name=="if" || gramsent.front().name=="while" )){//如果是语句，则进行语句判断
            int id=state_ment_num;
            state_ment_num++, statement.push_back(tree);
            statement.at(state_ment_num).children.clear();
            statement.at(state_ment_num).name="<语句>";
            state_string(na_state);
            if(!istreerror){
                statement.at(id).children.push_back(statement.at(id+1));
                statement.pop_back(), state_ment_num--;
            }

            if(gramsent.front().name!="end"){
                if(!istreerror){
                    if(!gramsent.empty() && gramsent.front().name==";") statement.at(state_ment_num).pushtree_node(";");
                    else out_error(0, " ; ");
                }
                if(!istreerror){
                    int id=state_ment_num;
                    state_ment_num++, statement.push_back(tree);
                    statement.at(state_ment_num).children.clear();
                    statement.at(state_ment_num).name="<语句表>";

                    state_ment(na_state);
                    if(!istreerror){
                        statement.at(id).children.push_back(statement.at(id+1));
                        statement.pop_back(), state_ment_num--;
                    }
                }
            }
        }else out_error(2, " ; ");
    }
}


void out_state_string()
{
    state_initial();
    state_ment_num++, statement.push_back(tree);
    statement.at(0).children.clear(), statement.at(0).name="<语句表>";
    state_ment("statestring");
}
