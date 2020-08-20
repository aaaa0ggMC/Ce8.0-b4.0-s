#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <windows.h>

using namespace std;

void split(vector<string> & vct,const string & line,const char sep);
string Trim(string & str);

class CeObject{
public:
    string m_name;
    string m_value;
    CeObject(string name,string value);
};
template<class T> class Data{
public:
    vector<T> m_data;
    vector<string> m_temp;
    virtual bool addItem(T v) = 0;
};
class CeData : public Data<CeObject>{
public:
    bool load(string fileName);
    bool load(LPCSTR fileName);
    vector<CeObject> _exportData();
    virtual bool addItem(CeObject v);
private:
    string m_file;
};

#endif // MAIN_H_INCLUDED
