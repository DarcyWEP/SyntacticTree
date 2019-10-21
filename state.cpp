#include <bits/stdc++.h>
#include "statestring.cpp"
using namespace std;

void loop_state()//<循环语句> -> while　<布尔表达式>　do　<语句>
{
    if(!istreerror){///<循环语句> -> while
        if(!gramsent.empty() && gramsent.front().name=="while") loopstate.at(loop_state_num).pushtree_node("while");
        else out_error(0, "(while)");
    }
    if(!istreerror){///<循环语句> - <布尔表达式>
        if(!gramsent.empty()){
            creatboolexpress();
            if(!istreerror){
                loopstate.at(loop_state_num).children.push_back(bool_express.at(0));//<条件语句> - <布尔表达式>
                bool_express.clear(), bool_express_num=-1;
            }
        }else out_error(0, "布尔表达式");//报错
    }
    if(!istreerror){///<循环语句> -> do
        if(!gramsent.empty() && gramsent.front().name=="do") loopstate.at(loop_state_num).pushtree_node("do");
        else out_error(0, "(do)");
    }
    if(!istreerror){
        if(!gramsent.empty()) bool_state_ment("loopstate");///<条件语句> - <语句>
        else out_error(0, "do后相应的表达式");
    }
}

void con_state()//<条件语句> -> if<布尔表达式>　then　<语句>｜if<布尔表达式>　then　<语句> 　else <语句>
{
    if(!istreerror){///<条件语句> - if
        if(!gramsent.empty() && gramsent.front().name=="if") constate.at(con_state_num).pushtree_node("if");
        else out_error(0, "(if)");
    }
    if(!istreerror){///<条件语句> - <布尔表达式>
        if(!gramsent.empty()){
            creatboolexpress();
            if(!istreerror){
                constate.at(con_state_num).children.push_back(bool_express.at(0));//<条件语句> - <布尔表达式>
                bool_express.clear(), bool_express_num=-1;
            }
        }else out_error(0, "布尔表达式");//报错
    }
    if(!istreerror){///<条件语句> - then
        if(!gramsent.empty() && gramsent.front().name=="then") constate.at(con_state_num).pushtree_node("then");
        else out_error(0, "(then)");
    }
    if(!istreerror){
        if(!gramsent.empty()) bool_state_ment("constate");///<条件语句> - <语句>
        else out_error(0, "if后相应的表达式");
    }

    if(!istreerror){///<条件语句> - else
        if(!gramsent.empty() && gramsent.front().name=="else"){
            constate.at(con_state_num).pushtree_node("else");
            if(!gramsent.empty() && gramsent.front().name!="end") bool_state_ment("constate");
            else{
                out_error(0, "else后相应的表达式");
                return;
            }
        }
        else return;
    }
}

void com_state()//< 复合语句> -> begin <语句串>end
{
    if(!istreerror){///< 复合语句> -> begin
        if(!gramsent.empty() && gramsent.front().name=="begin") comstate.at(com_state_num).pushtree_node("begin");
        else out_error(0, "(begin)");
    }
    if(!istreerror){
        int id=state_ment_num;
        state_ment_num++, statement.push_back(tree);
        statement.at(state_ment_num).children.clear();
        statement.at(state_ment_num).name="<语句表>";
        state_ment("comstate");///< 复合语句> - <语句串>
        if(!istreerror){
            comstate.at(com_state_num).children.push_back(statement.at(id+1));
            statement.pop_back(), state_ment_num--;
        }

    }

    if(!istreerror){///< 复合语句> - end
        if(!gramsent.empty() && gramsent.front().name=="end") comstate.at(com_state_num).pushtree_node("end");
        else out_error(0, "(end)");
    }
}

void ass_tate()//<赋值语句> -> id := <算术表达式>
{
    if(!istreerror){//<赋值语句> - id
        if(!gramsent.empty() && gramsent.front().clas==1){
            if( is_have_this_var(gramsent.front().name) ){//存在变量
                tree.name="<变量>", tree.children.clear();
                tree.pushtree_node(gramsent.front().name);
                asstate.children.push_back(tree);
            }else out_error(15, "");//报错
        }
    }
    if(!istreerror){//<赋值语句> - :=
        if(!gramsent.empty() && gramsent.front().name==":="){
            asstate.pushtree_node(gramsent.front().name);
        }else out_error(15, "");//报错
    }
    if(!istreerror){//<赋值语句> - <算术表达式>
        if(!gramsent.empty()){
            express_num++, express.push_back(tree);
            express.at(express_num).children.clear();
            express.at(express_num).name="<算术表达式>";
            expression();
            if(!istreerror){
                asstate.children.push_back(express.at(0));//<赋值语句> - <算术表达式>
                express.clear();
                express_num=-1;
            }
        }else out_error(0, "语句表达式");//报错
    }
}

void expression()//<算术表达式> -> <项>｜<项>+<算术表达式> | <项>-<算术表达式>
{
    if(!istreerror){
        if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2
                                 || gramsent.front().name=="(")){//符合则往下找项
            int id=nape_num;
            nape_num++ , nape.push_back(tree);
            nape.at(nape_num).children.clear(), nape.at(nape_num).name="<项>";
            creatnape();
            if(!istreerror){
                express.at(express_num).children.push_back(nape.at(id+1));///<算术表达式> - <项>
                nape.pop_back(), nape_num--;
                if(!gramsent.empty() && (gramsent.front().name=="+" || gramsent.front().name=="-") ){
                    express.at(express_num).pushtree_node(gramsent.front().name);///<算术表达式> - +(-)
                    int id=express_num;
                    express_num++, express.push_back(tree);
                    express.at(express_num).children.clear();
                    express.at(express_num).name="<算术表达式>";
                    expression();///<算术表达式> -> <算术表达式>
                    if(!istreerror){
                        express.at(id).children.push_back(express.at(id+1));
                        express.pop_back(), express_num--;
                    }
                }
            }
        }else out_error(2, "+ 或 -");
    }
}

void creatnape()//<项> -> <因子>｜<因子>*<项> | <因子>/<项>
{
    if(!istreerror){
        if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2 || gramsent.front().name=="(")){
            int id=factor_num;
            factor_num++, factor.push_back(tree);
            factor.at(factor_num).name="<因子>";
            factor.at(factor_num).children.clear();
            creatfactor();
            if(!istreerror){///<项> - <因子>
                nape.at(nape_num).children.push_back(factor.at(id+1));
                factor.pop_back(), factor_num--;
            }

            if(!istreerror){
                if(!gramsent.empty() && (gramsent.front().name=="*" || gramsent.front().name=="/") ){
                    nape.at(nape_num).pushtree_node(gramsent.front().name);///<项> - *(/)
                    if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2 || gramsent.front().name=="(") ){///<项> - <项>
                        int id=nape_num;
                        nape_num++, nape.push_back(tree);
                        nape.at(nape_num).name="<项>";
                        nape.at(nape_num).children.clear();
                        creatnape();
                        if(!istreerror){///<项> - <项>
                            nape.at(id).children.push_back(nape.at(id+1));
                            nape.pop_back();
                            nape_num--;
                        }
                    }else out_error(2, "* 或 /");
                }
            }
        }
    }

}

void creatfactor()//<因子> -> (<算术表达式>) | <变量> | <常数>
{
    if(!istreerror){
        if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2) ){
            if(gramsent.front().clas==1){///<因子> - <变量>
                if( is_have_this_var(gramsent.front().name) ){
                    tree.name="<变量>", tree.children.clear();
                    tree.pushtree_node(gramsent.front().name);
                    factor.at(factor_num).children.push_back(tree);
                }else out_error(15, "");
            }else{///<因子> - <常数>
                tree.name="<常数>", tree.children.clear();
                tree.pushtree_node(gramsent.front().name);
                factor.at(factor_num).children.push_back(tree);
            }
        }else if(!gramsent.empty() && gramsent.front().name=="(" ){
            factor.at(factor_num).pushtree_node(gramsent.front().name);///<因子> -> (
            if(!istreerror){
                int id=express_num;
                express_num++, express.push_back(tree);
                express.at(express_num).children.clear();
                express.at(express_num).name="<算术表达式>";
                expression();///<因子> -> <算术表达式>
                if(!istreerror){
                    factor.at(factor_num).children.push_back(express.at(id+1));
                    express.pop_back();//弹出最后一个
                    express_num--;
                }
                if(!istreerror){
                    if(!gramsent.empty() && gramsent.front().name==")" ){
                        factor.at(factor_num).pushtree_node(gramsent.front().name);///<因子> -> )
                    }else out_error(0, "')'");
                }
            }

        }
    }
}

void creatboolexpress()
{
    if(!istreerror){
        if(!gramsent.empty() && (gramsent.front().name=="!" || gramsent.front().clas==1 ||
                    gramsent.front().clas==2|| gramsent.front().name=="(") ){//是否满足布尔表达式的条件
            bool_express_num++, bool_express.push_back(tree);
            bool_express.at(bool_express_num).children.clear();
            bool_express.at(bool_express_num).name="<布尔表达式>";
            bool_expression();
        }else out_error(0, "布尔表达式");
    }
}
void bool_expression()//<布尔表达式> -> <关系表达式> | ！<布尔表达式> | <布尔表达式> && <布尔表达式>  | <布尔表达式> ||　<布尔表达式>
{
    if(!istreerror){
        if(!gramsent.empty() && gramsent.front().name=="!"){
            bool_express.at(bool_express_num).pushtree_node("!");///<布尔表达式> - !
            int id=bool_express_num;
            bool_express_num++, bool_express.push_back(tree);
            bool_express.at(bool_express_num).children.clear();
            bool_express.at(bool_express_num).name="<布尔表达式>";
            bool_expression();
            if(!istreerror){
                bool_express.at(id).children.push_back(bool_express.at(id+1));
                bool_express_num--, bool_express.pop_back();
            }
        }else if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2
                                       || gramsent.front().name=="(") ){
            int id=relation_express_num;
            relation_express_num++, relation_express.push_back(tree);
            relation_express.at(relation_express_num).children.clear();
            relation_express.at(relation_express_num).name="<关系表达式>";
            relationexpress();
            if(!istreerror){
                if(!gramsent.empty() && (gramsent.front().name=="&&" || gramsent.front().name=="||") ){
                    tree.children.clear(), tree.name="<布尔表达式>", tree.children.push_back(relation_express.at(id+1));
                    bool_express.at(bool_express_num).children.push_back(tree);///<布尔表达式> - <布尔表达式>
                    relation_express.pop_back(), relation_express_num--;
                    bool_express.at(bool_express_num).pushtree_node(gramsent.front().name);///<布尔表达式> - &&(||)
                    int id=bool_express_num;
                    bool_express_num++, bool_express.push_back(tree);
                    bool_express.at(bool_express_num).children.clear();
                    bool_express.at(bool_express_num).name="<布尔表达式>";
                    bool_expression();
                    if(!istreerror){
                        bool_express.at(id).children.push_back(bool_express.at(id+1));///<布尔表达式> - <布尔表达式>
                        bool_express_num--, bool_express.pop_back();
                    }
                }else if(!gramsent.empty()){
                    bool_express.at(bool_express_num).children.push_back(relation_express.at(id+1));///<布尔表达式> - <关系表达式>
                    relation_express.pop_back(), relation_express_num--;
                }
            }
        }else out_error(0, "布尔表达式");
    }
}

void relationexpress()//< 关系表达式> -> <算术表达式><关系><算术表达式>
{
    express.clear(), express_num=-1;
    nape.clear(), nape_num=-1;
    factor.clear(), factor_num=-1;

    if(!istreerror){
        express_num++, express.push_back(tree);
        express.at(express_num).children.clear();
        express.at(express_num).name="<算术表达式>";
        expression();
        if(!istreerror){
            relation_express.at(relation_express_num).children.push_back(express.at(0));//<赋值语句> - <算术表达式>
            express.clear();
            express_num=-1;
        }
    }
    if(!istreerror){
        string na=gramsent.front().name;
        if(!gramsent.empty() && (na=="<" || na=="<=" || na==">" || na==">=" || na=="=" || na=="<>") ){
            tree.name="<关系>", tree.children.clear(), tree.pushtree_node(na);//<关系> -
            relation_express.at(relation_express_num).children.push_back(tree);//< 关系表达式> - <关系>
        }else out_error(0, "关系符合");
    }
    if(!istreerror){
        express_num++, express.push_back(tree);
        express.at(express_num).children.clear();
        express.at(express_num).name="<算术表达式>";
        expression();
        if(!istreerror){
            relation_express.at(relation_express_num).children.push_back(express.at(0));//<赋值语句> - <算术表达式>
            express.clear();
            express_num=-1;
        }
    }
}
