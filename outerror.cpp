#include <bits/stdc++.h>
using namespace std;
bool istreerror=false;
int error_id=1;//�������
void out_error(int id, string error)//idΪ��������ͣ�10�Ժ��Ϊ������󣩣� errorΪ����ȱ�ٵĶ���
{
    istreerror=true;
    if(id==1) cout << "��" << error_id << "�л��" << error_id+1 <<  "�п���ȱ�ٱ�ʶ�������飡����" << endl;
    else if(id==0) cout << "��" << error_id <<  "�п���ȱ��" << error <<"�����飡����" << endl;
    else if(id==2) cout << "��" << error_id <<  "�п��ܶ��" << error <<"�����飡����" << endl;
    else if(id==12) cout << "var��Ŀ��ܳ��ֲ���ʶ����ַ������飡����" << endl;
    else if(id==13) cout << "��var��������������ʹ������飡����" << endl;
    else if(id==14) cout << "ȱ�ٳ��������־(end)��(.)" << endl;
    else if(id==15) cout <<  "��" << error_id << "�д���δ����ı���������" << endl;
}
