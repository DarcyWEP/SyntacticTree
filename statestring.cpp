#include <bits/stdc++.h>
#include "lexicalanalysis.cpp"
#include "outerror.cpp"
using namespace std;
map<string, bool> varmap;//����ı�����
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
            tree_node tr;//����root���ӽڵ���
            tr.name=na;
            tr.trey=gramsent.front();
            tr.children.clear();
            children.push_back(tr);//��ʼ��������
            gramsent.pop_front();//����ǵ���
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
        out_error(0, "��β��־(end)��(;)");
        return false;
    }
}
tree_node tree;

tree_node statestring;//<��䴮> -> <���>|<���>;<��䴮>
vector<tree_node> statement;//<���> -> <�������>|<��ֵ���>|<�������>|<ѭ�����>
vector<tree_node> comstate;//< �������> -> begin <��䴮>end
tree_node asstate;//<��ֵ���> -> id:=<���ʽ>
vector<tree_node> constate;//<�������> -> if<�������ʽ>��then��<���>��if<�������ʽ>��then��<���> ��else <���>
vector<tree_node> loopstate;//<ѭ�����> -> while��<�������ʽ>��do��<���>
vector<tree_node> express;// <�������ʽ> -> <��>��<��>+<�������ʽ> | <��>-<�������ʽ>
vector<tree_node> nape;//<��> -> <����>��<����>*<��> | <����>/<��>
vector<tree_node> factor;//<����> -> (<�������ʽ>) | <����> | <����>
vector<tree_node> bool_express;//<�������ʽ> -> <��ϵ���ʽ> | ��<�������ʽ> | <�������ʽ> && <�������ʽ>  | <�������ʽ> ||��<�������ʽ>
vector<tree_node> relation_express;//< ��ϵ���ʽ> -> <�������ʽ><��ϵ><�������ʽ>
int is_have_left=0;
void state_initial()
{
    statement.clear();
    nape.clear();
    factor.clear();
    bool_express.clear();
    relation_express.clear();
    statestring.name="<��䴮>";
    statestring.children.clear();
    comstate.clear();
    constate.clear();
    loopstate.clear();
    asstate.name="<��ֵ���>";
    asstate.children.clear();
    express.clear();
}

void state_ment(string na_state);//<��䴮> -> <���> | <���> ; <��䴮>
void bool_state_ment(string na_state);
void state_string(string na_state);//<���> -> <�������>|<��ֵ���>|<�������>|<ѭ�����>
void ass_tate();//<��ֵ���> -> id:=<���ʽ>
void expression();// <���ʽ> -> (<���ʽ>)��<����>+<����> | <����>-<����> | <����>*<����> | <����>/<����>
void bool_expression();//��ϵ���ʽ�еı��ʽ
void creatboolexpress();
void con_state();//<�������> -> if<�������ʽ>��then��<���>��if<�������ʽ>��then��<���> ��else <���>
void com_state();//< �������> -> begin <��䴮>end
void loop_state();//<ѭ�����> -> while��<�������ʽ>��do��<���>
void creatnape();//������
void creatfactor();//��������
void relationexpress();//������ϵ���ʽ


int state_ment_num=-1;
int factor_num=-1;
int nape_num=-1;
int bool_express_num=-1;
int relation_express_num=-1;
int con_state_num=-1;//�������
int com_state_num=-1;
int loop_state_num=-1;
int express_num=-1;




void state_string(string na_state)//<���> -> <�������>|<��ֵ���>|<�������>|<ѭ�����>
{
    tree_node tr;
    if(!istreerror){
        if(!gramsent.empty()){
            if(gramsent.front().clas==1){//<���> - <��ֵ���>
                asstate.name="<��ֵ���>", asstate.children.clear();
                statestring.name="<���>", statestring.children.clear();
                ass_tate();
                if(!istreerror) statement.at(state_ment_num).children.push_back(asstate);///<���> -> <��ֵ���>

            }else if(gramsent.front().name=="begin"){//<���> - <�������>
                if(na_state=="statestring") comstate.clear();//��һ��������䣬�뽫ǰ������
                com_state_num++;//��¼�����׵ĵڼ����������
                comstate.push_back(tr);
                comstate.at(com_state_num).name="<�������>";
                comstate.at(com_state_num).children.clear();
                com_state();
                if(!istreerror){
                    statement.at(state_ment_num).children.push_back(comstate.at(com_state_num));//<���> -> <��ֵ���>
                    com_state_num--; comstate.pop_back();
                }

            }else if(gramsent.front().name=="if"){//<���> - <�������>
                if(na_state=="statestring") constate.clear();//��һ��������䣬�뽫ǰ������
                con_state_num++;//��¼�����׵ĵڼ����������
                constate.push_back(tr);
                constate.at(con_state_num).name="<�������>";
                constate.at(con_state_num).children.clear();
                con_state();

                if(!istreerror){
                    statement.at(state_ment_num).children.push_back(constate.at(con_state_num));//<��䴮> -> <�������>
                    con_state_num--; constate.pop_back();
                }

            }else if(gramsent.front().name=="while"){//<��䴮> - <ѭ�����>
                if(na_state=="statestring") loopstate.clear();//��һ��������䣬�뽫ǰ������
                loop_state_num++;//��¼�����׵ĵڼ����������
                loopstate.push_back(tr);
                loopstate.at(loop_state_num).name="<ѭ�����>";
                loopstate.at(loop_state_num).children.clear();
                loop_state();

                if(!istreerror){
                    statement.at(state_ment_num).children.push_back(loopstate.at(loop_state_num));//<��䴮> -> <ѭ�����>
                    loop_state_num--; loopstate.pop_back();
                }
            }
        }
    }
}

void bool_state_ment(string na_state)
{
    if(!istreerror){//<��䴮> -> <���>|<���>;<��䴮>
        if(!gramsent.empty() && ( gramsent.front().clas==1 || gramsent.front().name=="begin"
                || gramsent.front().name=="if" || gramsent.front().name=="while" )){//�������䣬���������ж�
            int id=state_ment_num;
            state_ment_num++, statement.push_back(tree);
            statement.at(state_ment_num).children.clear();
            statement.at(state_ment_num).name="<���>";
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
    if(!istreerror){//<��䴮> -> <���>|<���>;<��䴮>
        if(!gramsent.empty() && ( gramsent.front().clas==1 || gramsent.front().name=="begin"
                || gramsent.front().name=="if" || gramsent.front().name=="while" )){//�������䣬���������ж�
            int id=state_ment_num;
            state_ment_num++, statement.push_back(tree);
            statement.at(state_ment_num).children.clear();
            statement.at(state_ment_num).name="<���>";
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
                    statement.at(state_ment_num).name="<����>";

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
    statement.at(0).children.clear(), statement.at(0).name="<����>";
    state_ment("statestring");
}
