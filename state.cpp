#include <bits/stdc++.h>
#include "statestring.cpp"
using namespace std;

void loop_state()//<ѭ�����> -> while��<�������ʽ>��do��<���>
{
    if(!istreerror){///<ѭ�����> -> while
        if(!gramsent.empty() && gramsent.front().name=="while") loopstate.at(loop_state_num).pushtree_node("while");
        else out_error(0, "(while)");
    }
    if(!istreerror){///<ѭ�����> - <�������ʽ>
        if(!gramsent.empty()){
            creatboolexpress();
            if(!istreerror){
                loopstate.at(loop_state_num).children.push_back(bool_express.at(0));//<�������> - <�������ʽ>
                bool_express.clear(), bool_express_num=-1;
            }
        }else out_error(0, "�������ʽ");//����
    }
    if(!istreerror){///<ѭ�����> -> do
        if(!gramsent.empty() && gramsent.front().name=="do") loopstate.at(loop_state_num).pushtree_node("do");
        else out_error(0, "(do)");
    }
    if(!istreerror){
        if(!gramsent.empty()) bool_state_ment("loopstate");///<�������> - <���>
        else out_error(0, "do����Ӧ�ı��ʽ");
    }
}

void con_state()//<�������> -> if<�������ʽ>��then��<���>��if<�������ʽ>��then��<���> ��else <���>
{
    if(!istreerror){///<�������> - if
        if(!gramsent.empty() && gramsent.front().name=="if") constate.at(con_state_num).pushtree_node("if");
        else out_error(0, "(if)");
    }
    if(!istreerror){///<�������> - <�������ʽ>
        if(!gramsent.empty()){
            creatboolexpress();
            if(!istreerror){
                constate.at(con_state_num).children.push_back(bool_express.at(0));//<�������> - <�������ʽ>
                bool_express.clear(), bool_express_num=-1;
            }
        }else out_error(0, "�������ʽ");//����
    }
    if(!istreerror){///<�������> - then
        if(!gramsent.empty() && gramsent.front().name=="then") constate.at(con_state_num).pushtree_node("then");
        else out_error(0, "(then)");
    }
    if(!istreerror){
        if(!gramsent.empty()) bool_state_ment("constate");///<�������> - <���>
        else out_error(0, "if����Ӧ�ı��ʽ");
    }

    if(!istreerror){///<�������> - else
        if(!gramsent.empty() && gramsent.front().name=="else"){
            constate.at(con_state_num).pushtree_node("else");
            if(!gramsent.empty() && gramsent.front().name!="end") bool_state_ment("constate");
            else{
                out_error(0, "else����Ӧ�ı��ʽ");
                return;
            }
        }
        else return;
    }
}

void com_state()//< �������> -> begin <��䴮>end
{
    if(!istreerror){///< �������> -> begin
        if(!gramsent.empty() && gramsent.front().name=="begin") comstate.at(com_state_num).pushtree_node("begin");
        else out_error(0, "(begin)");
    }
    if(!istreerror){
        int id=state_ment_num;
        state_ment_num++, statement.push_back(tree);
        statement.at(state_ment_num).children.clear();
        statement.at(state_ment_num).name="<����>";
        state_ment("comstate");///< �������> - <��䴮>
        if(!istreerror){
            comstate.at(com_state_num).children.push_back(statement.at(id+1));
            statement.pop_back(), state_ment_num--;
        }

    }

    if(!istreerror){///< �������> - end
        if(!gramsent.empty() && gramsent.front().name=="end") comstate.at(com_state_num).pushtree_node("end");
        else out_error(0, "(end)");
    }
}

void ass_tate()//<��ֵ���> -> id := <�������ʽ>
{
    if(!istreerror){//<��ֵ���> - id
        if(!gramsent.empty() && gramsent.front().clas==1){
            if( is_have_this_var(gramsent.front().name) ){//���ڱ���
                tree.name="<����>", tree.children.clear();
                tree.pushtree_node(gramsent.front().name);
                asstate.children.push_back(tree);
            }else out_error(15, "");//����
        }
    }
    if(!istreerror){//<��ֵ���> - :=
        if(!gramsent.empty() && gramsent.front().name==":="){
            asstate.pushtree_node(gramsent.front().name);
        }else out_error(15, "");//����
    }
    if(!istreerror){//<��ֵ���> - <�������ʽ>
        if(!gramsent.empty()){
            express_num++, express.push_back(tree);
            express.at(express_num).children.clear();
            express.at(express_num).name="<�������ʽ>";
            expression();
            if(!istreerror){
                asstate.children.push_back(express.at(0));//<��ֵ���> - <�������ʽ>
                express.clear();
                express_num=-1;
            }
        }else out_error(0, "�����ʽ");//����
    }
}

void expression()//<�������ʽ> -> <��>��<��>+<�������ʽ> | <��>-<�������ʽ>
{
    if(!istreerror){
        if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2
                                 || gramsent.front().name=="(")){//��������������
            int id=nape_num;
            nape_num++ , nape.push_back(tree);
            nape.at(nape_num).children.clear(), nape.at(nape_num).name="<��>";
            creatnape();
            if(!istreerror){
                express.at(express_num).children.push_back(nape.at(id+1));///<�������ʽ> - <��>
                nape.pop_back(), nape_num--;
                if(!gramsent.empty() && (gramsent.front().name=="+" || gramsent.front().name=="-") ){
                    express.at(express_num).pushtree_node(gramsent.front().name);///<�������ʽ> - +(-)
                    int id=express_num;
                    express_num++, express.push_back(tree);
                    express.at(express_num).children.clear();
                    express.at(express_num).name="<�������ʽ>";
                    expression();///<�������ʽ> -> <�������ʽ>
                    if(!istreerror){
                        express.at(id).children.push_back(express.at(id+1));
                        express.pop_back(), express_num--;
                    }
                }
            }
        }else out_error(2, "+ �� -");
    }
}

void creatnape()//<��> -> <����>��<����>*<��> | <����>/<��>
{
    if(!istreerror){
        if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2 || gramsent.front().name=="(")){
            int id=factor_num;
            factor_num++, factor.push_back(tree);
            factor.at(factor_num).name="<����>";
            factor.at(factor_num).children.clear();
            creatfactor();
            if(!istreerror){///<��> - <����>
                nape.at(nape_num).children.push_back(factor.at(id+1));
                factor.pop_back(), factor_num--;
            }

            if(!istreerror){
                if(!gramsent.empty() && (gramsent.front().name=="*" || gramsent.front().name=="/") ){
                    nape.at(nape_num).pushtree_node(gramsent.front().name);///<��> - *(/)
                    if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2 || gramsent.front().name=="(") ){///<��> - <��>
                        int id=nape_num;
                        nape_num++, nape.push_back(tree);
                        nape.at(nape_num).name="<��>";
                        nape.at(nape_num).children.clear();
                        creatnape();
                        if(!istreerror){///<��> - <��>
                            nape.at(id).children.push_back(nape.at(id+1));
                            nape.pop_back();
                            nape_num--;
                        }
                    }else out_error(2, "* �� /");
                }
            }
        }
    }

}

void creatfactor()//<����> -> (<�������ʽ>) | <����> | <����>
{
    if(!istreerror){
        if(!gramsent.empty() && (gramsent.front().clas==1 || gramsent.front().clas==2) ){
            if(gramsent.front().clas==1){///<����> - <����>
                if( is_have_this_var(gramsent.front().name) ){
                    tree.name="<����>", tree.children.clear();
                    tree.pushtree_node(gramsent.front().name);
                    factor.at(factor_num).children.push_back(tree);
                }else out_error(15, "");
            }else{///<����> - <����>
                tree.name="<����>", tree.children.clear();
                tree.pushtree_node(gramsent.front().name);
                factor.at(factor_num).children.push_back(tree);
            }
        }else if(!gramsent.empty() && gramsent.front().name=="(" ){
            factor.at(factor_num).pushtree_node(gramsent.front().name);///<����> -> (
            if(!istreerror){
                int id=express_num;
                express_num++, express.push_back(tree);
                express.at(express_num).children.clear();
                express.at(express_num).name="<�������ʽ>";
                expression();///<����> -> <�������ʽ>
                if(!istreerror){
                    factor.at(factor_num).children.push_back(express.at(id+1));
                    express.pop_back();//�������һ��
                    express_num--;
                }
                if(!istreerror){
                    if(!gramsent.empty() && gramsent.front().name==")" ){
                        factor.at(factor_num).pushtree_node(gramsent.front().name);///<����> -> )
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
                    gramsent.front().clas==2|| gramsent.front().name=="(") ){//�Ƿ����㲼�����ʽ������
            bool_express_num++, bool_express.push_back(tree);
            bool_express.at(bool_express_num).children.clear();
            bool_express.at(bool_express_num).name="<�������ʽ>";
            bool_expression();
        }else out_error(0, "�������ʽ");
    }
}
void bool_expression()//<�������ʽ> -> <��ϵ���ʽ> | ��<�������ʽ> | <�������ʽ> && <�������ʽ>  | <�������ʽ> ||��<�������ʽ>
{
    if(!istreerror){
        if(!gramsent.empty() && gramsent.front().name=="!"){
            bool_express.at(bool_express_num).pushtree_node("!");///<�������ʽ> - !
            int id=bool_express_num;
            bool_express_num++, bool_express.push_back(tree);
            bool_express.at(bool_express_num).children.clear();
            bool_express.at(bool_express_num).name="<�������ʽ>";
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
            relation_express.at(relation_express_num).name="<��ϵ���ʽ>";
            relationexpress();
            if(!istreerror){
                if(!gramsent.empty() && (gramsent.front().name=="&&" || gramsent.front().name=="||") ){
                    tree.children.clear(), tree.name="<�������ʽ>", tree.children.push_back(relation_express.at(id+1));
                    bool_express.at(bool_express_num).children.push_back(tree);///<�������ʽ> - <�������ʽ>
                    relation_express.pop_back(), relation_express_num--;
                    bool_express.at(bool_express_num).pushtree_node(gramsent.front().name);///<�������ʽ> - &&(||)
                    int id=bool_express_num;
                    bool_express_num++, bool_express.push_back(tree);
                    bool_express.at(bool_express_num).children.clear();
                    bool_express.at(bool_express_num).name="<�������ʽ>";
                    bool_expression();
                    if(!istreerror){
                        bool_express.at(id).children.push_back(bool_express.at(id+1));///<�������ʽ> - <�������ʽ>
                        bool_express_num--, bool_express.pop_back();
                    }
                }else if(!gramsent.empty()){
                    bool_express.at(bool_express_num).children.push_back(relation_express.at(id+1));///<�������ʽ> - <��ϵ���ʽ>
                    relation_express.pop_back(), relation_express_num--;
                }
            }
        }else out_error(0, "�������ʽ");
    }
}

void relationexpress()//< ��ϵ���ʽ> -> <�������ʽ><��ϵ><�������ʽ>
{
    express.clear(), express_num=-1;
    nape.clear(), nape_num=-1;
    factor.clear(), factor_num=-1;

    if(!istreerror){
        express_num++, express.push_back(tree);
        express.at(express_num).children.clear();
        express.at(express_num).name="<�������ʽ>";
        expression();
        if(!istreerror){
            relation_express.at(relation_express_num).children.push_back(express.at(0));//<��ֵ���> - <�������ʽ>
            express.clear();
            express_num=-1;
        }
    }
    if(!istreerror){
        string na=gramsent.front().name;
        if(!gramsent.empty() && (na=="<" || na=="<=" || na==">" || na==">=" || na=="=" || na=="<>") ){
            tree.name="<��ϵ>", tree.children.clear(), tree.pushtree_node(na);//<��ϵ> -
            relation_express.at(relation_express_num).children.push_back(tree);//< ��ϵ���ʽ> - <��ϵ>
        }else out_error(0, "��ϵ����");
    }
    if(!istreerror){
        express_num++, express.push_back(tree);
        express.at(express_num).children.clear();
        express.at(express_num).name="<�������ʽ>";
        expression();
        if(!istreerror){
            relation_express.at(relation_express_num).children.push_back(express.at(0));//<��ֵ���> - <�������ʽ>
            express.clear();
            express_num=-1;
        }
    }
}
