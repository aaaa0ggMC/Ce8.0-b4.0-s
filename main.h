/**************************************************************************
Copyright:aaaa0ggMC
Author:aaaa0ggMC
Date:2020-08-17
Description:main.cpp 头文件,声明了函数，类
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

int defaultSW = 0;//findAllSW stg的默认参数

//IFID -- 给每个ce中if语句的唯一的ID,还包含是否执行成功的信息
struct IFID{
public:
    ID id;//if语句的id
    static ID maxID;//id的最大大小
    /*
    *Summary:分配新的且唯一的id
    *Parameters:无
    *Return : 产生的ID
    */
    ID NewId();
    /*
    *Summary:构造方法，产生ID
    *Parameters:无
    *Return :无
    */
    IFID();
    /*
    *Summary:构造方法，产生ID，并获取结果
    *Parameters:
    *   result_:给的if语句的运行结果
    *Return :无
    */
    IFID(bool result_);
    bool result;//运行结果，用于检测else和else if语句
    /*
    *Summary:获取if的id
    *Parameters:无
    *Return :公共成员变量id
    */
    ID getId();
};
ID IFID::maxID = 0;//初始化最大id值

//PSStateID -- If 语句的独特id，无排序
struct PSStateID{
    string condition;
    string inside;
    PSStateID(string con,string in);
};

//PublicPair -- 更通用的ID
template<class T> struct PublicID{
    T & first;
    T & second;
};

//CeVariable -- Ce的变量
struct CeVariable{
public:
    string name;//变量的名字
    string value;//变量的值
    int depth;//作用域区域
    bool isconst;//变量是不是const的
    bool inorecase;//大小写
    bool out;//输出
    CeType type;
    /*
    *Summary:构造函数，用于生成变量
    *Parameters:
    *   vname:变量的名字
    *   vvalue:变量的值
    *   const_:变量是否是只读变量
    *Return :无
    */
    CeVariable(string vname,string vvalue,bool const_,int depth_ = 0,bool _inorecase = false);
    /*
    *Summary:设置变量的值
    *Parameters:
    *   v:变量的值
    *Return :返回一个int类型的值(6或0),6为该变量是const变量，不可修改，0表示没有异常
    *        但是如果赋给一个const变量相同的值，并不会报错
    */
    errType setValue(string v,bool api = false);
};

//CeMemory -- 系统内存
struct CeMemory{
public:
    vector<CeVariable> mem;//内存
    /*
    *Summary:构造方法，用于初始化内存
    *Parameters:无
    *Return :无
    */
    CeMemory();
    /*
    *Summary:检测内存中是否有一个变量叫vname
    *Parameters:
    *   vname:待检测变量的名字
    *Return :
    *   布尔类型值，返回内存中是否有该变量，有返回true,无返回false
    */
    bool has(string & vname);
    /*
    *Summary:获取变量在内存空间的位置
    *Parameters:
    *   vname:待获取值的变量的名字
    *Return :
    *   返回位置，如果返回-1，则表示没有这个变量
    */
    int getIndex(string & vname);
};

//Pos -- Position(位置)
//C风格
struct Pos{
    int st;//开始位置
    int ed;//结束位置
};

//BlockData -- 块信息
//C风格
struct BlockData{
    string head;//头部
    string inside;//内容
    char operators[3];//符号
    /*
    *Summary:初始化内容
    *Parameters:无
    *Return :无
    */
    void init();
};

//ProgramInfo -- 项目信息(在.ce中可以调用)
//C风格
struct ProgramInfo{
    string fileLocation;//文件位置
    string authorName;//作者名字
    unsigned long runTime;//现在的运行时间
};

typedef vector<PSStateID> TIfStates;
typedef TIfStates * PTIfStates;

vector<string> filer;//文件内容
const string spector = "--------------------------------------------";//分界线
bool isBreak = false;//是否用了break;语句
int ifdepth = 0;//if语句的深度
const errType varNotFound = 1,syntax = 2,isNotFunction = 3,oppCharNotHave = 4,/*noelse_if = 5,*/none = 0,constCannotSet = 6,
argNotEnough = 7,varReDefine = 8,varNotGood = 9,noAcess = 10,argNotGood = 11;
//各种错误类型
/*
*varNotFound:没有找到变量
*syntax:语法错误
*isNotFunction:不是函数
*oppCharNotHave:没有相应的转义字符
*noelse_if:ifdepth等于0,表示没有相应的if语句,现有else(或else if)，错误,修改成了忽略错误的块语句头
*none:无错误
*constCannotSet:无法设置const变量的值
*/
vector<string> blocks{"if","loop","rloop","while"};//块语句的标识
string constVar = "$%Const^%",externVar = "$%Extern^%",ifselVar = "$system$selectIF^%~",apiVar = "$api$useAPI^%~",
apiIVar = "$api$inoreCase^%~",apiOutVar = "$api$outable^%~";
int maxDepth = 0;
bool nosee = false;
vector<unsigned int> warings;
ProgramInfo info;
CONSOLE_SCREEN_BUFFER_INFO consoleinfoin,consoleinfoo,consoleinfoe;
bool checkApi = true;

string pdStr = "+ -({;";//判定的字符串

const WORD white = FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,
            red = FOREGROUND_INTENSITY|FOREGROUND_RED,green = FOREGROUND_INTENSITY|FOREGROUND_GREEN,
            blue = FOREGROUND_INTENSITY|FOREGROUND_BLUE,yellow = FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN,
            pink = FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE,bigGreen /*青色*/= FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE,
            orign = FOREGROUND_INTENSITY,backwhite = BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,
            backred = BACKGROUND_INTENSITY | BACKGROUND_RED,backgreen = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
            backblue = BACKGROUND_BLUE | BACKGROUND_INTENSITY,backyellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
            backpink = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,backBigGreen = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
            backorign = BACKGROUND_INTENSITY;//颜色
map<string,CeType> types{std::make_pair("TP_int",0),std::make_pair("TP_string",1)
                          ,std::make_pair("TP_var",2),std::make_pair("TP_float",3)};

/*
*Summary:解码，识别代码
*Parameters:
*   file:文件内容
*Return :无
*/
void decode(vector<string> & file);
/*
*Summary:去除前后导空白字符，以及'\0'
*Parameters:
*   str:待处理的字符串
*Return :str
*/
string Trim(string & str);
/*
*Summary:切割字符串成多个小字符串
*Parameters:
*   vct:待写入的容器
*   line:目标
*   sep:切割的字符
*Return :无
*/
void split(vector<string> & vct,const string & line,const char sep);
/*
*Summary:测试时用来把空白字符打印出来
*Parameters:
*   str:待打印的字符串
*Return :无
*/
void DebugPrint(string str);
/*
*Summary:处理错误
*Parameters:
*   type:定位错误的类型
*   line:定位检查到文件的行数
*   character:定位检查文件的第几个字符
*   add:附加字符串
*Return :无
*/
void errMessage(errType type,int line,int character,string add);
/*
*Summary:处理错误
*Parameters:
*   type:定位错误的类型
*   line:定位检查到文件的行数
*   character:定位检查文件的第几个字符
*Return :无
*/
void errMessage(errType type,int line,int character);
/*
*Summary:检查字符串中是否包含了某个字符
*Parameters:
*   c:字符
*   tar:待检测字符串
*Return :布尔类型，如果包含，返回true,否则返回false
*/
bool strInclude(char c,const string & tar);
/*
*Summary:查找字符串中是否有另一个字符串,但如果与到'\"'，则会跳至下一个'\"'，并继续检查
*Parameters:
*   tar:待查找字符串
*   cmp:查找的字符串
*   times:出现的次数
*Return :返回查找到的位置，当查找不到时，返回-1或-2
*/
int find(const string & tar,const string & cmp,int times);
/*
*Summary:预处理文件，目前只删除注释和在块语句的结尾多加一个'\n'(非常重要！)
*Parameters:
*   in:待写入容器
*   file:待读取容器
*Return :无
*/
void beforeRun(vector<string> & in,vector<string> & file);
/*
*Summary:单纯的获取字符串中某个字符出现的次数
*Parameters:
*   tar:待读取字符串
*   c:字符
*Return :返回出现的次数
*/
int getCharC(const string & tar,char c);
/*
*Summary:获取字符串中某个字符出现的次数，但如果与到'\"'，则会跳至下一个'\"'，并继续检查
*Parameters:
*   tar:待读取字符串
*   c:字符
*Return :返回出现的次数
*/
int ceGetCharC(const string & tar,char c);
/*
*Summary:和string.find_last_of作用几乎一模一样，自己品
*/
size_t findLastIndexOf(const string & str,const string & compare);
/*
*Summary:查找一对小括号块内的内容
*Parameters:
*   tar:待读取字符串
*   times:次数
*Return :位置
*/
Pos block_find_samllK(const string & tar,int times);
/*
*Summary:查找一对大括号块内的内容
*Parameters:
*   tar:待读取字符串
*   times:次数
*Return :位置
*/
Pos block_find_bigK(const string & tar,int times);
/*
*Summary:将字符串中的数据解析出来，并写入另一个字符串
*Parameters:
*   in:待写入字符串
*   tar:待读取字符串
*   line:执行到的行数（由decode的计数器给予）
*Return :返回是否读取成功，如果返回不是"",那么则表示不存在一个变量的名字叫（返回的值）
*/
string rsStr(string & in,const string & tar,int line = 0);
/*
*Summary:把文件中的字符串转化为输出的字符串，如"\\n"转成"\n"...
*Parameters:
*   in:待写入字符串
*   str:待读取字符串
*   rtV:结果，当它不为-1时，表示识别转义字符错误
*Return :无
*/
void toCeStr(string & in,string & str,int * rtV);
/*
*Summary:获取布尔语句的结果(支持&&,||)
*Parameters:
*   condition:语句
*   errMsg:错误的信息，如果它为""，表示没有错误，否则表示condition没有一个变量叫errMsg
*Return :结果(一般0或1)，但当它返回-1时，表示出问题了，需要检测errMsg
*/
int getConditionResult(string condition,string & errMsg);
/*
*Summary:获取布尔语句的结果
*Parameters:
*   condition:语句
*   errMsg:错误的信息，如果它为""，表示没有错误，否则表示condition没有一个变量叫errMsg
*Return :结果(一般0或1)，但当它返回-1时，表示出问题了，需要检测errMsg
*/
int getSMConditionResult(string condition,string & errMsg);
/*
*Summary:Ce的核心之一，运行decode()分解出来的小的代码(块)
*Parameters:
*   f:待解析字符串
*   line:行数
*   blockOpition:附加的信息
*   ifStates:用于处理if语句的附加项
*Return :处理值
*/
string code(const string & f,int line,string blockOpition,PTIfStates ifStates = NULL,bool api = false,string varNameADD = "");
/*
*Summary:Ce的核心之一，分解出小的代码(块)
*Parameters:
*   file:整个大文件
*Return :无
*/
void decode(vector<string> & file);
/*
*Summary:装载系统提供的.rb(Ce预处理过后的文件)文件，用来辅助运行
*Parameters:无
*Return :无
*/
void loadSystem();
/*
*Summary:转化字符串为整型变量
*Parameters:
*   str:目标字符串
*Return :转化的结果
*/
int toInt(std::string & str);
/*
*Summary:获得标题
*Parameters:
*   line:句子
*   gv:附加搜索
*Return :标题
*/
string getTitle(string line,string gv = pdStr);
/*
*Summary:获得块的内容
*Parameters:
*   type:类型,0表示小括号，1表示大括号
*   line:句子
*Return :结果
*/
BlockData getBlockData(int type,string line);
/*
*Summary:清除相应作用域的块
*Parameters:
*   mem:内存空间
*   depth:作用域层数
*Return :结果
*/
void removeBlockVariable(CeMemory & mem,int depth);
/*
*Summary:查找完整的cmp在不在tar中
*Parameters:
*   tar:目标
*   cmp:比较字符串
*   times:出现次数
*   stg:获取检查开始的字符串的位置
*Return :结果
*/
string::size_type findAllSW(string tar,string cmp,int * stg = &defaultSW);
/*
*Summary:排序
*Parameters:
*   read:待读取目标
*Return :结果
*/
template<class T> vector<T> sortVector(vector<T> read);
/*
*Summary:查找最大项
*Parameters:
*   bg:待读取目标
*Return :结果
*/
int findBiggest(vector<int> bg);
/*
*Summary:查找最内部的括号内的项
*Parameters:
*   tar:待读取目标
*Return :结果
*/
Pos findInsidestC(string tar);
/*
*Summary:替换str
*Parameters:
*   tar:待读取目标
*   token:目标
*   rp:替换的东西
*Return :结果
*/
string replaceStr(string tar,string token,string rp);
/*
*Summary:扩展string的加法
*Parameters:
*   ***
*Return :结果
*/
string operator+(string a,string b);
string operator+(const char * a,string b);
string operator+(string b,const char * a);
/*
*Summary:大写
*Parameters:
*   ***
*Return :结果
*/
string uppercase(string tar);
/*
*Summary:编译时运行预处理(毕竟:Ce是解释性语言)
*Parameters:
*   args:参数
*Return :错误类型
*/
errType use_pragma(vector<string> args);
/*
*Summary:时间
*Parameters:
*   args:参数
*Return :无
*/
void * timeable(void * arg);
/*
*Summary:windows的事件处理
*Parameters:
*   flags:样式
*Return :结果
*/
BOOL loginCtrl(DWORD flags);
/*
*Summary:检测忽略警告的id
*Parameters:
*   v:值
*Return :结果
*/
bool warsHas(unsigned int v);
/*
*Summary:多彩的打印一串文字
*Parameters:
*   meaage:信息
*   color:颜色
*Return :打印的字数(printf返回)
*/
int colorfulPrint(const string message,WORD color);
/*
*Summary:打印一串红色文字
*Parameters:
*   meaage:信息
*Return :打印的字数(printf返回)
*/
int errPrint(const string message);
/*
*Summary:打印一串蓝色文字
*Parameters:
*   meaage:信息
*Return :打印的字数(printf返回)
*/
int warPrint(const string message);
/*
*Summary:变量的命名法则
*Parameters:
*   meaage:信息
*Return :错误
*/
errType varRule(string varName);
/*
*Summary:使用api
*Parameters:
*   key:api的权限
*   funcName:函数名
*   funcArgs:函数的参数
*   return_:处理值
*Return :错误
*/
errType apiUse(int key,string funcName,vector<string> funcArgs,string & return_);

#endif // MAIN_H_INCLUDED
