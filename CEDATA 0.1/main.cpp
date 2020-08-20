#include "main.h"

using namespace std;

int main()
{
    CeData data;
    data.load("test.cedt");
    for(const auto i : data._exportData()){
        cout << i.m_name << " " << i.m_value << endl;
    }
    system("pause");
    return 0;
}

string Trim(string & str){
    string blanks("\f\v\r\t\n ");
    string temp;
    for(int i = 0;i < (int)str.length();i++){
        if(str[i] == '\0')
            str[i] = '\t';
        else if(blanks.find(str[i]) == string::npos)
            break;
    }
    str.erase(0,str.find_first_not_of(blanks));
    str.erase(str.find_last_not_of(blanks) + 1);
    temp = str;
    return temp;
}

void split(vector<string> & vct,const string & line,const char sep){
    const size_t size = line.size();
    const char* str = line.c_str();
    int start = 0,end = 0;
    for(int i = 0;i < (int)size;i++){
        if(str[i] == sep){
            string st = line.substr(start,end);
            Trim(st);
            vct.push_back(st);
            start = i + 1;
            end = 0;
        }else
            end++;
    }
    if(end > 0){
        string st = line.substr(start,end);
        Trim(st);
        vct.push_back(st);
    }
}

bool CeData::load(LPCSTR fileName){
    FILE * file;
    char ch;
    vector<string> temp;
    if((file = fopen(fileName,"r")) == NULL){
        printf("加载一个叫%s的文件失败!!!\n",fileName);
        return false;
    }
    while((ch = getc(file)) != EOF){
        m_file += ch;
    }
    if(fclose(file) != 0){
        printf("关闭一个叫%s的文件失败!!!\n",fileName);
        return false;
    }
    split(m_temp,m_file,';');
    for(int i = 0;i < (int)m_temp.size();i++){
        Trim(m_temp[i]);
    }
    for(const auto i : m_temp){
        if(i.compare("") == 0){
            continue;
        }
        temp.clear();
        split(temp,i,':');
        if(!addItem(CeObject(temp[0],temp[1]))){
            cout << "--------------------------" << endl;
            cout << "变量重名错误!!!!" << endl;
            cout << i << endl;
            cout << "--------------------------" << endl;
            return false;
        }
    }
    return true;
}

bool CeData::load(string fileName){return load((LPCSTR)fileName.c_str());}

bool CeData::addItem(CeObject v){
    for(const CeObject i : m_data){
        if(i.m_name.compare(v.m_name) == 0)
            return false;
    }
    m_data.push_back(v);
    return true;
}

CeObject::CeObject(string name,string value)
:m_name(name)
,m_value(value){

}

vector<CeObject> CeData::_exportData(){
    vector<CeObject> vec;
    for(auto i : m_data){
        vec.push_back(i);
    }
    return vec;
}
