/**************************************************************************
Copyright:aaaa0ggMC
Author:aaaa0ggMC
Date:2020-08-17
Description:main.cpp ͷ�ļ�,�����˺�������
**************************************************************************/
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#define _GLIBCXX_USE_CXX11_ABI 0
#define WINVER 0x10086
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>
#include <ctype.h>
#include <string>
#include <map>
#include <algorithm>
#include <functional>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

using namespace std;

#define delay Sleep(500)

#define Version "alpha 0.0.8.0"
#define UpdateTime "2020/8/17"

typedef unsigned int ID;
typedef int errType;
typedef unsigned int CeType;

int defaultSW = 0;//findAllSW stg��Ĭ�ϲ���

//IFID -- ��ÿ��ce��if����Ψһ��ID,�������Ƿ�ִ�гɹ�����Ϣ
struct IFID{
public:
    ID id;//if����id
    static ID maxID;//id������С
    /*
    *Summary:�����µ���Ψһ��id
    *Parameters:��
    *Return : ������ID
    */
    ID NewId();
    /*
    *Summary:���췽��������ID
    *Parameters:��
    *Return :��
    */
    IFID();
    /*
    *Summary:���췽��������ID������ȡ���
    *Parameters:
    *   result_:����if�������н��
    *Return :��
    */
    IFID(bool result_);
    bool result;//���н�������ڼ��else��else if���
    /*
    *Summary:��ȡif��id
    *Parameters:��
    *Return :������Ա����id
    */
    ID getId();
};
ID IFID::maxID = 0;//��ʼ�����idֵ

//PSStateID -- If ���Ķ���id��������
struct PSStateID{
    string condition;
    string inside;
    PSStateID(string con,string in);
};

//PublicPair -- ��ͨ�õ�ID
template<class T> struct PublicID{
    T & first;
    T & second;
};

//CeVariable -- Ce�ı���
struct CeVariable{
public:
    string name;//����������
    string value;//������ֵ
    int depth;//����������
    bool isconst;//�����ǲ���const��
    bool inorecase;//��Сд
    bool out;//���
    CeType type;
    /*
    *Summary:���캯�����������ɱ���
    *Parameters:
    *   vname:����������
    *   vvalue:������ֵ
    *   const_:�����Ƿ���ֻ������
    *Return :��
    */
    CeVariable(string vname,string vvalue,bool const_,int depth_ = 0,bool _inorecase = false);
    /*
    *Summary:���ñ�����ֵ
    *Parameters:
    *   v:������ֵ
    *Return :����һ��int���͵�ֵ(6��0),6Ϊ�ñ�����const�����������޸ģ�0��ʾû���쳣
    *        �����������һ��const������ͬ��ֵ�������ᱨ��
    */
    errType setValue(string v,bool api = false);
};

//CeMemory -- ϵͳ�ڴ�
struct CeMemory{
public:
    vector<CeVariable> mem;//�ڴ�
    /*
    *Summary:���췽�������ڳ�ʼ���ڴ�
    *Parameters:��
    *Return :��
    */
    CeMemory();
    /*
    *Summary:����ڴ����Ƿ���һ��������vname
    *Parameters:
    *   vname:��������������
    *Return :
    *   ��������ֵ�������ڴ����Ƿ��иñ������з���true,�޷���false
    */
    bool has(string & vname);
    /*
    *Summary:��ȡ�������ڴ�ռ��λ��
    *Parameters:
    *   vname:����ȡֵ�ı���������
    *Return :
    *   ����λ�ã��������-1�����ʾû���������
    */
    int getIndex(string & vname);
};

//Pos -- Position(λ��)
//C���
struct Pos{
    int st;//��ʼλ��
    int ed;//����λ��
};

//BlockData -- ����Ϣ
//C���
struct BlockData{
    string head;//ͷ��
    string inside;//����
    char operators[3];//����
    /*
    *Summary:��ʼ������
    *Parameters:��
    *Return :��
    */
    void init();
};

//ProgramInfo -- ��Ŀ��Ϣ(��.ce�п��Ե���)
//C���
struct ProgramInfo{
    string fileLocation;//�ļ�λ��
    string authorName;//��������
    unsigned long runTime;//���ڵ�����ʱ��
};

typedef vector<PSStateID> TIfStates;
typedef TIfStates * PTIfStates;

vector<string> filer;//�ļ�����
const string spector = "--------------------------------------------";//�ֽ���
bool isBreak = false;//�Ƿ�����break;���
int ifdepth = 0;//if�������
const errType varNotFound = 1,syntax = 2,isNotFunction = 3,oppCharNotHave = 4,/*noelse_if = 5,*/none = 0,constCannotSet = 6,
argNotEnough = 7,varReDefine = 8,varNotGood = 9,noAcess = 10,argNotGood = 11;
//���ִ�������
/*
*varNotFound:û���ҵ�����
*syntax:�﷨����
*isNotFunction:���Ǻ���
*oppCharNotHave:û����Ӧ��ת���ַ�
*noelse_if:ifdepth����0,��ʾû����Ӧ��if���,����else(��else if)������,�޸ĳ��˺��Դ���Ŀ����ͷ
*none:�޴���
*constCannotSet:�޷�����const������ֵ
*/
vector<string> blocks{"if","loop","rloop","while"};//�����ı�ʶ
string constVar = "$%Const^%",externVar = "$%Extern^%",ifselVar = "$system$selectIF^%~",apiVar = "$api$useAPI^%~",
apiIVar = "$api$inoreCase^%~",apiOutVar = "$api$outable^%~";
int maxDepth = 0;
bool nosee = false;
vector<unsigned int> warings;
ProgramInfo info;
CONSOLE_SCREEN_BUFFER_INFO consoleinfoin,consoleinfoo,consoleinfoe;
bool checkApi = true;

string pdStr = "+ -({;";//�ж����ַ���

const WORD white = FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,
            red = FOREGROUND_INTENSITY|FOREGROUND_RED,green = FOREGROUND_INTENSITY|FOREGROUND_GREEN,
            blue = FOREGROUND_INTENSITY|FOREGROUND_BLUE,yellow = FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN,
            pink = FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE,bigGreen /*��ɫ*/= FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE,
            orign = FOREGROUND_INTENSITY,backwhite = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,
            backred = BACKGROUND_INTENSITY | BACKGROUND_RED,backgreen = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
            backblue = BACKGROUND_BLUE | BACKGROUND_INTENSITY,backyellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
            backpink = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,backBigGreen = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
            backorign = BACKGROUND_INTENSITY;//��ɫ
map<string,CeType> types{std::make_pair("TP_int",0),std::make_pair("TP_string",1)
                          ,std::make_pair("TP_var",2),std::make_pair("TP_float",3)};

/*
*Summary:���룬ʶ�����
*Parameters:
*   file:�ļ�����
*Return :��
*/
void decode(vector<string> & file);
/*
*Summary:ȥ��ǰ�󵼿հ��ַ����Լ�'\0'
*Parameters:
*   str:��������ַ���
*Return :str
*/
string Trim(string & str);
/*
*Summary:�и��ַ����ɶ��С�ַ���
*Parameters:
*   vct:��д�������
*   line:Ŀ��
*   sep:�и���ַ�
*Return :��
*/
void split(vector<string> & vct,const string & line,const char sep);
/*
*Summary:����ʱ�����ѿհ��ַ���ӡ����
*Parameters:
*   str:����ӡ���ַ���
*Return :��
*/
void DebugPrint(string str);
/*
*Summary:�������
*Parameters:
*   type:��λ���������
*   line:��λ��鵽�ļ�������
*   character:��λ����ļ��ĵڼ����ַ�
*   add:�����ַ���
*Return :��
*/
void errMessage(errType type,int line,int character,string add);
/*
*Summary:�������
*Parameters:
*   type:��λ���������
*   line:��λ��鵽�ļ�������
*   character:��λ����ļ��ĵڼ����ַ�
*Return :��
*/
void errMessage(errType type,int line,int character);
/*
*Summary:����ַ������Ƿ������ĳ���ַ�
*Parameters:
*   c:�ַ�
*   tar:������ַ���
*Return :�������ͣ��������������true,���򷵻�false
*/
bool strInclude(char c,const string & tar);
/*
*Summary:�����ַ������Ƿ�����һ���ַ���,������뵽'\"'�����������һ��'\"'�����������
*Parameters:
*   tar:�������ַ���
*   cmp:���ҵ��ַ���
*   times:���ֵĴ���
*Return :���ز��ҵ���λ�ã������Ҳ���ʱ������-1��-2
*/
int find(const string & tar,const string & cmp,int times);
/*
*Summary:Ԥ�����ļ���Ŀǰֻɾ��ע�ͺ��ڿ����Ľ�β���һ��'\n'(�ǳ���Ҫ��)
*Parameters:
*   in:��д������
*   file:����ȡ����
*Return :��
*/
void beforeRun(vector<string> & in,vector<string> & file);
/*
*Summary:�����Ļ�ȡ�ַ�����ĳ���ַ����ֵĴ���
*Parameters:
*   tar:����ȡ�ַ���
*   c:�ַ�
*Return :���س��ֵĴ���
*/
int getCharC(const string & tar,char c);
/*
*Summary:��ȡ�ַ�����ĳ���ַ����ֵĴ�����������뵽'\"'�����������һ��'\"'�����������
*Parameters:
*   tar:����ȡ�ַ���
*   c:�ַ�
*Return :���س��ֵĴ���
*/
int ceGetCharC(const string & tar,char c);
/*
*Summary:��string.find_last_of���ü���һģһ�����Լ�Ʒ
*/
size_t findLastIndexOf(const string & str,const string & compare);
/*
*Summary:����һ��С���ſ��ڵ�����
*Parameters:
*   tar:����ȡ�ַ���
*   times:����
*Return :λ��
*/
Pos block_find_samllK(const string & tar,int times);
/*
*Summary:����һ�Դ����ſ��ڵ�����
*Parameters:
*   tar:����ȡ�ַ���
*   times:����
*Return :λ��
*/
Pos block_find_bigK(const string & tar,int times);
/*
*Summary:���ַ����е����ݽ�����������д����һ���ַ���
*Parameters:
*   in:��д���ַ���
*   tar:����ȡ�ַ���
*   line:ִ�е�����������decode�ļ��������裩
*Return :�����Ƿ��ȡ�ɹ���������ز���"",��ô���ʾ������һ�����������ֽУ����ص�ֵ��
*/
string rsStr(string & in,const string & tar,int line = 0);
/*
*Summary:���ļ��е��ַ���ת��Ϊ������ַ�������"\\n"ת��"\n"...
*Parameters:
*   in:��д���ַ���
*   str:����ȡ�ַ���
*   rtV:�����������Ϊ-1ʱ����ʾʶ��ת���ַ�����
*Return :��
*/
void toCeStr(string & in,string & str,int * rtV);
/*
*Summary:��ȡ�������Ľ��(֧��&&,||)
*Parameters:
*   condition:���
*   errMsg:�������Ϣ�������Ϊ""����ʾû�д��󣬷����ʾconditionû��һ��������errMsg
*Return :���(һ��0��1)������������-1ʱ����ʾ�������ˣ���Ҫ���errMsg
*/
int getConditionResult(string condition,string & errMsg);
/*
*Summary:��ȡ�������Ľ��
*Parameters:
*   condition:���
*   errMsg:�������Ϣ�������Ϊ""����ʾû�д��󣬷����ʾconditionû��һ��������errMsg
*Return :���(һ��0��1)������������-1ʱ����ʾ�������ˣ���Ҫ���errMsg
*/
int getSMConditionResult(string condition,string & errMsg);
/*
*Summary:Ce�ĺ���֮һ������decode()�ֽ������С�Ĵ���(��)
*Parameters:
*   f:�������ַ���
*   line:����
*   blockOpition:���ӵ���Ϣ
*   ifStates:���ڴ���if���ĸ�����
*Return :����ֵ
*/
string code(const string & f,int line,string blockOpition,PTIfStates ifStates = NULL,bool api = false,string varNameADD = "");
/*
*Summary:Ce�ĺ���֮һ���ֽ��С�Ĵ���(��)
*Parameters:
*   file:�������ļ�
*Return :��
*/
void decode(vector<string> & file);
/*
*Summary:װ��ϵͳ�ṩ��.rb(CeԤ���������ļ�)�ļ���������������
*Parameters:��
*Return :��
*/
void loadSystem();
/*
*Summary:ת���ַ���Ϊ���ͱ���
*Parameters:
*   str:Ŀ���ַ���
*Return :ת���Ľ��
*/
int toInt(std::string & str);
/*
*Summary:��ñ���
*Parameters:
*   line:����
*   gv:��������
*Return :����
*/
string getTitle(string line,string gv = pdStr);
/*
*Summary:��ÿ������
*Parameters:
*   type:����,0��ʾС���ţ�1��ʾ������
*   line:����
*Return :���
*/
BlockData getBlockData(int type,string line);
/*
*Summary:�����Ӧ������Ŀ�
*Parameters:
*   mem:�ڴ�ռ�
*   depth:���������
*Return :���
*/
void removeBlockVariable(CeMemory & mem,int depth);
/*
*Summary:����������cmp�ڲ���tar��
*Parameters:
*   tar:Ŀ��
*   cmp:�Ƚ��ַ���
*   times:���ִ���
*   stg:��ȡ��鿪ʼ���ַ�����λ��
*Return :���
*/
string::size_type findAllSW(string tar,string cmp,int * stg = &defaultSW);
/*
*Summary:����
*Parameters:
*   read:����ȡĿ��
*Return :���
*/
template<class T> vector<T> sortVector(vector<T> read);
/*
*Summary:���������
*Parameters:
*   bg:����ȡĿ��
*Return :���
*/
int findBiggest(vector<int> bg);
/*
*Summary:�������ڲ��������ڵ���
*Parameters:
*   tar:����ȡĿ��
*Return :���
*/
Pos findInsidestC(string tar);
/*
*Summary:�滻str
*Parameters:
*   tar:����ȡĿ��
*   token:Ŀ��
*   rp:�滻�Ķ���
*Return :���
*/
string replaceStr(string tar,string token,string rp);
/*
*Summary:��չstring�ļӷ�
*Parameters:
*   ***
*Return :���
*/
string operator+(string a,string b);
string operator+(const char * a,string b);
string operator+(string b,const char * a);
/*
*Summary:��д
*Parameters:
*   ***
*Return :���
*/
string uppercase(string tar);
/*
*Summary:����ʱ����Ԥ����(�Ͼ�:Ce�ǽ���������)
*Parameters:
*   args:����
*Return :��������
*/
errType use_pragma(vector<string> args);
/*
*Summary:ʱ��
*Parameters:
*   args:����
*Return :��
*/
void * timeable(void * arg);
/*
*Summary:windows���¼�����
*Parameters:
*   flags:��ʽ
*Return :���
*/
BOOL loginCtrl(DWORD flags);
/*
*Summary:�����Ծ����id
*Parameters:
*   v:ֵ
*Return :���
*/
bool warsHas(unsigned int v);
/*
*Summary:��ʵĴ�ӡһ������
*Parameters:
*   meaage:��Ϣ
*   color:��ɫ
*Return :��ӡ������(printf����)
*/
int colorfulPrint(const string message,WORD color);
/*
*Summary:��ӡһ����ɫ����
*Parameters:
*   meaage:��Ϣ
*Return :��ӡ������(printf����)
*/
int errPrint(const string message);
/*
*Summary:��ӡһ����ɫ����
*Parameters:
*   meaage:��Ϣ
*Return :��ӡ������(printf����)
*/
int warPrint(const string message);
/*
*Summary:��������������
*Parameters:
*   meaage:��Ϣ
*Return :����
*/
errType varRule(string varName);
/*
*Summary:ʹ��api
*Parameters:
*   key:api��Ȩ��
*   funcName:������
*   funcArgs:�����Ĳ���
*   return_:����ֵ
*Return :����
*/
errType apiUse(int key,string funcName,vector<string> funcArgs,string & return_);

#endif // MAIN_H_INCLUDED
