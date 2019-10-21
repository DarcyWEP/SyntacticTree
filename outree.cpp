#include <bits/stdc++.h>
#include "treefirst.cpp"

using namespace std;
vector<int> cnt_flag;
ofstream oftree;
void opentree(){ oftree.open(("outree.txt")); cnt_flag.clear(); }
void outree(tree_node tr, int width, bool pop);
/*void outree2(tree_node tr, int width)
{
    for(int i=0; i<width; i++) oftree << "   ";
    oftree << tr.name << " -> " ;
    for(int i=0; i<(int)tr.children.size(); i++) oftree << tr.children.at(i).name << " ";
    oftree << endl;
    for(int i=0; i<(int)tr.children.size(); i++){
        if(tr.children.at(i).children.size()!=0){
            outree2(tr.children.at(i), width+1);
        }
    }
}*/

void outree(tree_node tr, int width, bool pop)
{
    if(width!=0){
        bool is_have_this=false;
        for(int i=0; i<(int)cnt_flag.size(); i++){
            if(cnt_flag.at(i)==width) is_have_this=true;
        }
        if(!is_have_this){
            cnt_flag.push_back(width);
            //sort(cnt_flag.begin(), cnt_flag.end() );
        }
    }
    int cnt_flag_it=0;
    if(tr.name!="<³ÌÐò>")
        for(int i=0; i<=width; i++){
            if( i==cnt_flag.at(cnt_flag_it) ){
                cnt_flag_it++;
                oftree << "|";
            }
            else oftree << " ";
    }
    if(pop) cnt_flag.pop_back();
    if( (int)tr.children.size()>1 ){
        oftree << tr.name << "|" << endl;
        for(int i=0; i<(int)tr.children.size(); i++){
            if(tr.name!="<³ÌÐò>"){
                if(i==(int)tr.children.size()-1) outree(tr.children.at(i), tr.name.length()+1+width, true);
                else outree(tr.children.at(i), tr.name.length()+1+width, false);
            }
            else{
                if(i==(int)tr.children.size()-1) outree(tr.children.at(i), tr.name.length()+width, true);
                else outree(tr.children.at(i), tr.name.length()+width, false);
            }
        }
    }else if((int)tr.children.size()==1){
        oftree << tr.name << "->";
        if((int)tr.children.at(0).children.size()==0) oftree << tr.children.at(0).name << endl;
        else if((int)tr.children.at(0).children.size()==1){
            tree_node tree_tr;
            tree_tr=tr.children.at(0).children.at(0);
            width=width+tr.name.length()+2;
            oftree << tr.children.at(0).name << "->";
            width+=tr.children.at(0).name.length()+2;
            while(1){
                if((int)tree_tr.children.size()==0){
                    oftree << tree_tr.name << endl;
                    break;
                }else if((int)tree_tr.children.size()==1){
                    oftree << tree_tr.name << "->";
                    width=width+tree_tr.name.length()+2;
                    tree_tr=tree_tr.children.at(0);
                }else{
                    oftree << tree_tr.name << "|" << endl;
                    width=width+tree_tr.name.length()+1;
                    for(int i=0; i<(int)tree_tr.children.size(); i++){
                        if(i==(int)tree_tr.children.size()-1) outree(tree_tr.children.at(i), width, true);
                        else outree(tree_tr.children.at(i), width, false);
                    }
                    break;
                }
            }
        }else{
            oftree << tr.children.at(0).name << "|" << endl;
            for(int i=0; i<(int)tr.children.at(0).children.size(); i++){
                if(i==(int)tr.children.at(0).children.size()-1) outree(tr.children.at(0).children.at(i), tr.children.at(0).name.length()+tr.name.length()+3+width, true);
                else outree(tr.children.at(0).children.at(i), tr.children.at(0).name.length()+tr.name.length()+3+width, false);
            }
        }
    }else if( (int)tr.children.size()==0 ){
        oftree << tr.name << endl;
    }
}

void out_tree()
{
    opentree();
    //outree2(root, 0);
    outree(root, 0, false);
}
