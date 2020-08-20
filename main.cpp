/**************************************************************************
Copyright:aaaa0ggMC
Author:aaaa0ggMC
Date:2020-08-17
Description:实现了main.cpp的函数，类
**************************************************************************/
#include "main.h"

using namespace std;

vector<IFID> ifids;
CeMemory memory;
pthread_t timeT;

int main(int argc,char * argv[])
{
    fstream stream;
    bool outfile = false;
    srand(time(0)*time(0));
    vector<string> tempFile;
    if(argc < 2){
        errPrint("错误：参数不够！！！我们至少要一个参数！！\n提示:键入\"ce -help\"来获得帮助！\n");
        Sleep(1000);
        return 0;
    }
    if(!strcmp("-help",argv[1]) || !strcmp("-?",argv[1])){
        colorfulPrint("使用 \"ce [目标文件/模式]\"\n",yellow);
        delay;
        colorfulPrint("-help    / -?   :帮助\n"
                      "-version / -ver :版本\n",yellow);
        delay;
        colorfulPrint("之后，系统便会启动它！\n",yellow);
        delay;
        colorfulPrint("记得，文件格式必须是ANSI，否则一些非英文字符便会乱码！\n",yellow);
        delay;
        return 0;
    }else if(!strcmp("-version",argv[1]) || !strcmp("-ver",argv[1])){
        char sr;
        string str = "start ";
        colorfulPrint(spector+"\n",green);
        colorfulPrint("版本:" Version "\n",yellow);
        delay;
        colorfulPrint("作者:aaaa0ggMc\n",yellow);
        delay;
        colorfulPrint("网上:https://space.bilibili.com/394099679\n",yellow);
        delay;
        colorfulPrint("访问(按下[ENTER]便是不去)?",yellow);
        delay;
        sr = getchar();
        while((sr != '\n')){
            if(getchar() == '\n')
                break;
        }
        str += "https://space.bilibili.com/394099679";
        if(sr != '\n'){
            system(str.c_str());
            delay;
            MessageBox(NULL,(LPCSTR)"谢谢访问！！！",(LPCSTR)"系统",MB_OK | MB_TOPMOST);
        }
        delay;
        colorfulPrint("更新时间:" UpdateTime " 是最晚的更新\n",yellow);
        colorfulPrint(spector+"\n",green);
        delay;
        return 0;
    }
    if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)loginCtrl,TRUE)){
        warPrint("waring:A,oh,Can't set onExitFunc!!!\n警告:A，Oh,无法设置退出函数!!!\n");
    }
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&consoleinfoo);
    GetConsoleScreenBufferInfo(GetStdHandle(STD_INPUT_HANDLE),&consoleinfoin);
    GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),&consoleinfoe);
    loadSystem();
    string filePath = "";
    string pth = argv[1];
    filePath += argv[1];
    if(filePath.find_last_of('.') != string::npos)
        filePath = filePath.substr(filePath.find_last_of('.'),filePath.length()-filePath.find_last_of('.'));
    else{
        filePath = ".rb";
        pth += ".rb";
    }
    Trim(filePath);
    if(filePath.compare(".ce") != 0){
        if(filePath.compare(".rb") == 0){
            outfile = true;
        }else{
            errPrint("文件后缀名不是\"ce\",请您更改后缀名!!!\n");
            Sleep(1000);
            return 0;
        }
    }
    stream.open(/*"E:/coding/C/CppProbject/ce/bin/Debug/1.rb"*/pth,ios::in);
    if(!stream.is_open()){
        errPrint("打开一个叫"+pth+"失败！\n");
        Sleep(1000);
        return 0;
    }
    while (!stream.eof())
    {
        string strtemp;
        strtemp.resize(1024);
        stream.getline((char *)strtemp.c_str(),1024);
        Trim(strtemp);
        strtemp.resize(strlen(strtemp.c_str()) + 1);//success
        tempFile.push_back(strtemp);
    }
    stream.close();
    if(stream.is_open()){
        errPrint("关闭一个叫"+pth+"失败！\n");
        Sleep(1000);
        return 0;
    }
    if(!outfile)
        beforeRun(filer,tempFile);//预处理
    else
        filer.insert(filer.end(), tempFile.begin(), tempFile.end());
    int dp = maxDepth++;
    info.fileLocation.resize(MAX_PATH);
    GetCurrentDirectory(MAX_PATH,(LPSTR)info.fileLocation.c_str());
    string::size_type loc = 0,old;
    for(int i = 0;i < getCharC(info.fileLocation,'\\');i++){
        old = loc;
        loc = info.fileLocation.find('\\',old+2);
        if(loc == string::npos)break;
        info.fileLocation.insert(loc+1,"\\");
    }
    Trim(info.fileLocation);
    memory.mem.push_back(CeVariable("Directory__",info.fileLocation,true,-999));
    memory.mem.push_back(CeVariable("runtime__",to_string(0),true,-999));
    memory.mem.push_back(CeVariable("author__",info.authorName,true,-999));
    pthread_create(&timeT,NULL,timeable,NULL);
    pthread_detach(timeT);
    decode(filer);//解码
    maxDepth--;
    removeBlockVariable(memory,dp);
    if(nosee && !(warsHas(0) || warsHas(1))){
        warPrint("\nwaring:A,oh,Your notes have no ending!!!\nChecking your notes now!!!\n"
                 "警告:A，Oh,你的注释没有结束!!!\n现在检查你的注释吧!!!(如果你对结果有疑惑的话)\n");
    }
    pthread_cancel(timeT);
    return 0;
}

ID IFID::NewId(){
    return ++IFID::maxID;
}

IFID::IFID(bool result_){
    this->id = this->NewId();
    this->result = result_;
}

IFID::IFID(){
  this->id = this->NewId();
}

ID IFID::getId(){
    ID temp = this->id;
    return temp;
}

CeVariable::CeVariable(string vname,string vvalue,bool const_,int depth_,bool _inorecase){
    isconst = const_;
    name = vname;
    value = vvalue;
    depth = depth_;
    inorecase = _inorecase;
}

errType CeVariable::setValue(string v,bool api){
    if(isconst && v.compare(value) != 0 && !api)
        return constCannotSet;
    value = v;
    return none;
}

CeMemory::CeMemory(){}

bool CeMemory::has(string & vname){
    for(unsigned int i = 0;i < mem.size();i++){
        if(mem[i].inorecase){
            if(uppercase(vname).compare(uppercase(mem[i].name)) == 0)
                return true;
        }
        if(vname.compare(mem[i].name) == 0)
            return true;
    }
    return false;
}

int CeMemory::getIndex(string & vname){
    for(unsigned int i = 0;i < mem.size();i++){
        if(mem[i].inorecase){
            if(uppercase(vname).compare(uppercase(mem[i].name)) == 0)
                return (int)i;
        }
        if(vname.compare(mem[i].name) == 0)
            return (int)i;
    }
    return -1;
}

PSStateID::PSStateID(string con,string in){
    this->condition = con;
    this->inside = in;
}

void BlockData::init(){
    head = inside = "";
    operators[0] = operators[1] = operators[2] = '\0';
}


string Trim(string & str){
    string blanks("\f\v\r\t\n ");
    string temp;
    for(int i = 0;i < (int)str.length();i++){
        if(str[i] == '\0')
            str[i] = '\t';
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

void DebugPrint(string str){
    for(int i = 0;i < (int)str.length();i++){
        switch(str[i]){
        case '\n':
            cout << "\\n";
            continue;
        case '\t':
            cout << "\\t";
            continue;
        case '\f':
            cout << "\\f";
            continue;
        case '\v':
            cout << "\\v";
            continue;
        case '\a':
            cout << "\\a";
            continue;
        case '\r':
            cout << "\\r";
            continue;
        case '\b':
            cout << "\\b";
            continue;
        case '\0':
            cout << "\\0";
            continue;
        default:
            cout << str[i];
            continue;
        }
    }
}


void errMessage(errType type,int line,int character,string add){
    switch(type){
    case varNotFound:
        errPrint("错误，没有一个变量叫做\""+add+"\"\n");
        break;
    case syntax:
        errPrint("语法错误！！！\n");
        break;
    case isNotFunction:
        errPrint("\""+add+"\"是一个函数而非关键字!!!!\n");
        break;
    case oppCharNotHave:
        errPrint("错误，没有一个反义符号叫\"\\"+add+"\"\n");
        break;
    case none:
        return;
    case constCannotSet:
        errPrint("无法修改const变量\""+add+"\"的值\n");
        break;
    case argNotEnough:
        errPrint("参数不够！！！要"+add+"个才行！！！\n");
        break;
    case varReDefine:
        errPrint("变量"+add+"已定义！！！！\n");
        break;
    case varNotGood:
        errPrint("变量不符合命名法则!!!\n");
        break;
    case noAcess:
        errPrint("你没有权限调用API关键字"+add+",添加CESTDAPI标签在"+add+"之前用来获得权限!!!\n");
        break;
    case argNotGood:
        errPrint("你给的值不对！！！！！！！！"+add+"\n");
        break;
    }
    errPrint("在第"+to_string(line)+"行，第 "+to_string(character)+"个字符。\n");
    errPrint("请检查您的.rb看看是否编译，或者修改.ce代码，再编译，再运行\n");
    delay;
    pthread_cancel(timeT);
    exit(type);
}

void errMessage(errType type,int line,int character){
    errMessage(type,line,character,"");
}

bool strInclude(char c,const string & tar){
    for(unsigned int i = 0;i < tar.length();i++){
        if(tar[i] == '\0')
            continue;
        if(c == tar[i])
            return true;
    }
    return false;
}

int find(const string & tar,const string & cmp,int times){
    int time = 0;
    int index = -1;
    if(tar.find_first_of(cmp) == string::npos){
        return -1;
    }
    for(index = 0;index < (int)tar.length() && time < times;index++){//先加index,再检查time
        if(tar[index] == '/' && tar[index+1] == '/'){
            return -2;
        }else if(tar[index] == '\\' && tar[index + 1] == '\"'){
            //do nothing
        }else if(tar[index] == '\"' && !strInclude('\"',cmp)){
            for(index++;index < (int)tar.length();index++){
                if(tar[index] == '\"'){
                    break;
                }
            }
        }else if(strInclude(tar[index],cmp)){
            time++;
        }
    }
    return index;
}

int getCharC(const string & tar,char c){
    int t = 0;
    for(unsigned int index = 0;index < tar.length();index++){
        if(tar[index] == c)
            t++;
    }
    return t;
}

int ceGetCharC(const string & tar,char c){
    int t = 0;
    int strTime = 0;
    for(unsigned int index = 0;index < tar.length();index++){
        if(tar[index] == '\\' && tar[index+1] == '\"'){
            index++;
            continue;
        }else if(tar[index] == '\"' && c != '\"'){
            strTime++;
            index = find(tar,"\"",strTime+1);
        }
        if(tar[index] == c)
            t++;
    }
    return t;
}

size_t findLastIndexOf(const string & str,const string & compare){
    size_t sizew = str.length();
    for(;sizew > 0;sizew--){
        if(strInclude(str[sizew-1],compare)){
            return sizew;
        }
    }
    return string::npos;
}

Pos block_find_samllK(const string & tar,int times){
    Pos ps;
    ps.st = find(tar,"(",times);
    int depth = 1;
    int i;
    for(i = ps.st;i < (int)tar.length();i++){
        if(tar[i] == '\"'){
            for(i++;i < (int)tar.length();i++){
                if(tar[i] == '\"'){
                    break;
                }
            }
        }
        if(tar[i] == '(')depth++;
        else if(tar[i] == ')')depth--;
        if(depth == 0)break;
    }
    if(depth == 1 && i > 0){
        i = -1;
    }
    ps.ed = i;
    return ps;
}

Pos block_find_bigK(const string & tar,int times){
    Pos ps;
    ps.st = find(tar,"{",times);
    int depth = 1;
    int i;
    for(i = ps.st;i < (int)tar.length();i++){
        if(tar[i] == '\"'){
            for(i++;i < (int)tar.length();i++){
                if(tar[i] == '\"'){
                    break;
                }
            }
        }
        if(tar[i] == '{')depth++;
        else if(tar[i] == '}')depth--;
        if(depth == 0)break;
    }
    if(depth == 1 && i > 0){
        i = -1;
    }
    ps.ed = i;
    return ps;
}


string rsStr(string & in,const string & tar,int line){//it hasnot encoding it to CeStr
    std::string temp = "";
    int index = 0;
    int odindex;
    int count = ceGetCharC(tar,'+');
    //cout << "target:" << tar << endl;
    if(count > 0){
        for(int u = 0;u < count+1;u++){
            odindex = index;
            index = find(tar,"+",u+1);
            if(u == count){
                index = tar.length()+1;
            }
            if(index < 0){
                break;
            }
            string str = tar.substr(odindex,index - odindex-1);
            Trim(str);
            if(str.c_str()[0] == '\"' && str.c_str()[str.length()-1] == '\"'){
                int rindex = str.find_first_of("\"") + 1;
                int rodindex = str.find_last_of("\"");
                temp += str.substr(rindex,rodindex - rindex);
            }else if(isdigit(str.c_str()[0]) || str.c_str()[0] == '+' || str.c_str()[0] == '-'){
                if((str.c_str()[0] == '+' || str.c_str()[0] == '-')){
                    if(str.length() < 2)
                        return to_string((char)str.c_str()[0]);
                    if(isdigit(str.c_str()[1])){
                        int strt = atoi(str.c_str());
                        temp += to_string(strt);
                    }
                    else
                        return str;
                }else{
                    int strt = atoi(str.c_str());
                    temp += to_string(strt);
                }
            }else{
                temp += code(str,line,"");
            }
        }
    }else{
        if(tar.c_str()[0] == '\"' && tar.c_str()[tar.length()-1] == '\"'){
            int rindex = tar.find_first_of("\"") + 1;
            int rodindex = tar.find_last_of("\"");
            temp = tar.substr(rindex,rodindex - rindex);
        }else if(isdigit(tar.c_str()[0]) || tar.c_str()[0] == '+' || tar.c_str()[0] == '-'){
            if((tar.c_str()[0] == '+' || tar.c_str()[0] == '-')){
                if(tar.length() < 2)
                    return to_string((char)tar.c_str()[0]);
                if(isdigit(tar.c_str()[1])){
                    int str = atoi(tar.c_str());
                    temp = to_string(str);
                }
                else
                    return tar;
            }else{
                int str = atoi(tar.c_str());
                temp = to_string(str);
            }
        }else{
            temp += code(tar,line,"");
        }
    }
    in = temp;
    return "";
}

void toCeStr(string & in,string & str,int * rtV){
    in = "";
    *rtV = -1;
    for(unsigned int i = 0;i < str.length();i++){
        if(str[i] == '\\'){
            switch(str[++i]){//先加后检查！！！，i++是先检查后加
            case 'n':
                in += '\n';
                break;
            case 'b':
                in += '\b';
                break;
            case 'r':
                in += '\r';
                break;
            case 't':
                in += '\t';
                break;
            case '\\':
                in += '\\';
                break;
            case 'f':
                in += '\f';
                break;
            case 'v':
                in += '\v';
                break;
            case 'k':
                in += ' ';
                break;
            case '/':
                in += '/';
                break;
            default:
                *rtV = i;
                return;
                break;
            }
            continue;
        }
        in += str[i];
    }
}

int getConditionResult(string condition,string & errMsg){
    int result;
    string right;
    bool con = false;
    vector<string> consR = {"&&","||"};
    for(int i = 0;i < (int)consR.size();i++){
        int index = condition.find(consR[i].c_str());
        if(index > 0){
            con = true;
            string left = condition.substr(0,index);
            Trim(left);
            right = condition.substr(index+consR[i].length(),condition.length()-index);
            Trim(right);
            int rt = getSMConditionResult(left,errMsg);
            if(rt < 0)
                return -1;
            result = rt;
            rt = getConditionResult(right,errMsg);
            if(rt < 0)
                return -1;
            if(consR[i].compare("&&") == 0){
                result &= rt;
            }else if(consR[i].compare("||") == 0){
                result |= rt;
            }
            break;
        }
    }
    if(!con){
        result = getSMConditionResult(condition,errMsg);
    }
    return result;
}

int getSMConditionResult(string condition,string & errMsg){
    const int maxSize = 6;
    string operators[maxSize]{"==","!=",">","<",">=","<="};
    for(int i = 0;i < maxSize;i++){
        int index = condition.find(operators[i].c_str());
        if(index > 0){
            string left = condition.substr(0,index);
            Trim(left);
            string right = condition.substr(index+operators[i].length(),condition.length()-index);
            Trim(right);
            string lv,rv;
            rsStr(lv,left);
            rsStr(rv,right);
            if(operators[i].compare("==") == 0){
                return (lv.compare(rv) == 0) ? 1:0;
            }else if(operators[i].compare("!=") == 0){
                return (lv.compare(rv) == 0) ? 0:1;
            }else if(operators[i].compare(">") == 0){
                int li = toInt(lv);
                int ri = toInt(rv);
                return (li > ri) ? 1:0;
            }else if(operators[i].compare("<") == 0){
                int li = toInt(lv);
                int ri = toInt(rv);
                return (li < ri) ? 1:0;
            }else if(operators[i].compare(">=") == 0){
                int li = toInt(lv);
                int ri = toInt(rv);
                return (li >= ri) ? 1:0;
            }else if(operators[i].compare("<=") == 0){
                int li = toInt(lv);
                int ri = toInt(rv);
                return (li <= ri) ? 1:0;
            }
        }
    }
    string v;
    string rt = rsStr(v,condition);
    if(rt.compare("") != 0){
        errMsg = condition;
        return -1;
    }
    return (v.compare("") == 0)?0:1;
}

string code(const string & f,int line,string blockOpition,PTIfStates ifStates,bool api,string varNameADD){
    Trim((string &)f);
    Trim(blockOpition);
    blockOpition += '\n';
    if(f.compare(";") == 0 || f.compare("") == 0){
        return "";
    }
    int index1 = f.find_first_of(pdStr),index2 = 0;
    std::string title = f.substr(0,index1 - 0);
    if(title.compare(";") == 0 || title.compare("") == 0){
        return "";
    }
    if(uppercase(f).compare("NOTESSTART") == 0){
        nosee = true;
        return "";
    }else if(uppercase(f).compare("NOTESEND") == 0){
        nosee = false;
        return "";
    }
    if(nosee){
        return "";
    }
    if(api){
        if(varNameADD.find('(') != string::npos){
            int m_api_use = 0;
            Pos _ps = block_find_samllK(varNameADD,1);
            string insi_ = varNameADD.substr(_ps.st,_ps.ed-_ps.st);
            string func,rt__;
            vector<string> funcArgs;
            if(find(varNameADD,":",1) >= 0){
                string ltit = getTitle(varNameADD,pdStr+":");
                if(ltit.compare("API") == 0)m_api_use = 1;
                func = replaceStr(varNameADD,ltit+":","");
                Trim(func);
                func = getTitle(func);
            }else
                func = varNameADD.substr(0,_ps.st-1);
            Trim(func);
            Trim(insi_);
            split(funcArgs,insi_,',');
            errMessage(apiUse(m_api_use,func,funcArgs,rt__),line,index1,rt__);
            varNameADD = rt__;
        }
        return varNameADD;
    }
    if(title.compare("print") == 0){
        //cout << "go print" << endl;
        index1 = f.find_first_of("(") + 1;
        index2 = (int)findLastIndexOf(f,")") - 1;//批注：以后添加注释功能，要修改一下
        int rt;
        string cestr = "";
        string subr = "";
        string rst = rsStr(subr,f.substr(index1,index2 - index1));//substr(start,length)
        if(rst != "")errMessage(varNotFound,line,index1,rst);
        toCeStr(cestr,subr,&rt);
        if(rt != -1)errMessage(oppCharNotHave,line,index1,to_string(f[rt]));
        cout << cestr;//don't need to add endl!!!!
        return to_string(cestr.length());
    }else if(title.compare("string") == 0 || title.compare("var") == 0
             || title.compare("int") == 0 || title.compare("float") == 0
             || title.compare("Number") == 0 || title.compare("BaseObj") == 0){
        index1 = index1 + 1;
        index2 = f.length();
        string all = f.substr(index1,index2 - index1);
        if(ceGetCharC(all,',') != 0){
            std::vector<string> def_vars;
            split(def_vars,(const string &)all,',');
            for(int i = 0;i < (int)def_vars.size();i++){
                string temp = title;
                temp += " ";
                temp += def_vars[i];
                code(temp,line,blockOpition);
            }
            return "";
        }
        index2 = f.find_first_of("=)") - 1;
        string varName = f.substr(index1,index2 - index1);
        Trim(varName);
        if(varName.find('(') != string::npos)
            varName = code(f,line,blockOpition,ifStates,true,f.substr(index1,index2 - index1+2));
        errMessage(varRule(varName),line,index1,varName);
        string value = "";
        if(strInclude('=',f)){
            index1 = f.find_first_of("=") + 1;
            index2 = f.length();
            string v = f.substr(index1,index2 - index1);
            Trim(v);
            string rst = rsStr(value,v);//substr(start,length)
            if(rst != "")errMessage(varNotFound,line,index1,rst);
        }
        if(memory.has(varName))errMessage(varReDefine,line,index1,varName);
        vector<string> sayings;
        CeVariable var = CeVariable(varName,value,false,maxDepth-1);
        bool isConst = false,isExtern = false,useApi = false,inorecase = false,isout = false;
        split(sayings,blockOpition,';');
        for(int i = 0;i < (int)sayings.size();i++){
            if(sayings[i].compare(constVar) == 0)isConst = true;
            else if(sayings[i].compare(externVar) == 0)isExtern = true;
            else if(sayings[i].compare(apiVar) == 0)useApi = true;
            else if(sayings[i].compare(apiIVar) == 0){
                if(useApi || !checkApi)inorecase = true;
                else errMessage(noAcess,line,index1,"INORECASE");
            }else if(sayings[i].compare(apiOutVar) == 0){
                if(useApi || !checkApi)isout = true;
                else errMessage(noAcess,line,index1,"OUTABLE");
            }
        }
        var.isconst = isConst;
        var.depth = (isExtern)?INT_MIN+1:maxDepth-1;
        var.inorecase = inorecase;
        var.out = isout;
        memory.mem.push_back(var);
        return var.value;
    }else if(title.compare("input") == 0){
        index1 = f.find_first_of("(") + 1;
        index2 = (int)findLastIndexOf(f,")") - 1;//批注：以后添加注释功能，要修改一下
        string cestr = "";
        string subr = f.substr(index1,index2 - index1);
        if(subr.find('(') != string::npos)
            subr = code(f,line,blockOpition,ifStates,true,subr);
        if(!memory.has(subr) && subr.compare("") != 0)errMessage(varNotFound,line,index1,subr);
        string rc = "";
        char ch;
        while((ch = getchar()) != '\n'){
            rc += ch;
        }
        if(subr == "" || subr == "NULL")return rc;
        errType msg = memory.mem[memory.getIndex(subr)].setValue(rc);
        errMessage(msg,line,index1,subr);
        return rc;
    }else if(title.compare("local") == 0){
        index1 = f.find_first_of("(") + 1;
        index2 = (int)findLastIndexOf(f,")") - 1;//批注：以后添加注释功能，要修改一下
        string cestr = "";
        string subr = f.substr(index1,index2 - index1);
        if(subr.find('(') != string::npos)
            subr = code(f,line,blockOpition,ifStates,true,subr);
        if(!memory.has(subr))errMessage(varNotFound,line,index1,subr);
        if(!memory.mem[memory.getIndex(subr)].out)errMessage(noAcess,line,index1, "使用local变量:"+memory.mem[memory.getIndex(subr)].name);
        string _v;
        _v.resize(1024);
        sprintf((char *)_v.c_str(),
               "variable name                :%s\n"
               "variable value               :%s\n"
               "variable virtual localtion   :0x%04X\n"
               "variable's attributes        :\n"
               "    const                    :%s\n"
               "    inorecase                :%s\n"
               "    depth                    :%d\n"
               ,memory.mem[memory.getIndex(subr)].name.c_str(),
               memory.mem[memory.getIndex(subr)].value.c_str(),
               memory.getIndex(subr),(memory.mem[memory.getIndex(subr)].isconst?"true":"false"),
               (memory.mem[memory.getIndex(subr)].inorecase?"true":"false"),
               memory.mem[memory.getIndex(subr)].depth);
        printf("%s",_v.c_str());
        return _v;
    }else if(title.compare("#pragma") == 0){
        vector<string> args;
        string str = replaceStr(f,title,"");
        Trim(str);
        split(args,str,' ');
        errMessage(use_pragma(args),line,index1,to_string(2));
        return args[0];
    }else if(title.compare("rand") == 0){
        index1 = f.find_first_of("(") + 1;
        index2 = (int)findLastIndexOf(f,")") - 1;//批注：以后添加注释功能，要修改一下
        string cestr = "";
        string subr = f.substr(index1,index2 - index1);
        if(ceGetCharC(subr,',') != 2)errMessage(argNotEnough,line,index1,"3");
        vector<string> args;
        split(args,subr,',');
        int stnum = atoi(args[0].c_str()),ednum = atoi(args[1].c_str());
        if(args[2].find('(') != string::npos)
            args[2] = code(f,line,blockOpition,ifStates,true,args[2]);
        if(ceGetCharC(args[2],'\"') != 0)errMessage(constCannotSet,line,index1,args[2]);
        if(!memory.has(args[2]))errMessage(varNotFound,line,index1,args[2]);
        int rnd = rand()%(ednum - stnum)+stnum;
        string rc = to_string(rnd);
        if(subr == "" || subr == "NULL")return rc;
        errType msg = memory.mem[memory.getIndex(args[2])].setValue(rc);
        errMessage(msg,line,index1,args[2]);
        srand(rnd);
        return rc;
    }else if(title.compare("break") == 0){
        isBreak = true;
    }else if(title.compare("exitCmd") == 0){
        int index1 = find(f,"(",1);
        if(index1 < 0)errMessage(isNotFunction,line,index1,"exitCmd");
        HWND win = GetConsoleWindow();
        pthread_cancel(timeT);
        PostMessage(win,WM_CLOSE,0,0);//直接关闭
    }else if(title.compare("exit") == 0){
        int index1 = find(f,"(",1);
        if(index1 < 0)errMessage(isNotFunction,line,index1,"exit");
        pthread_cancel(timeT);
        exit(0);
    }else if(title.compare("const") == 0){
        string temp = f.substr(index1,f.length());
        Trim(temp);
        code(temp,line,blockOpition+";"+constVar);
    }else if(title.compare("extern") == 0){
        string temp = f.substr(index1,f.length());
        Trim(temp);
        code(temp,line,blockOpition+";"+externVar);
    }else if(title.compare("CESTDAPI") == 0){
        string temp = f.substr(index1,f.length());
        Trim(temp);
        code(temp,line,blockOpition+";"+apiVar);
    }else if(title.compare("INORECASE") == 0){
        string temp = f.substr(index1,f.length());
        Trim(temp);
        code(temp,line,blockOpition+";"+apiIVar);
    }else if(title.compare("OUTABLE") == 0){
        string temp = f.substr(index1,f.length());
        Trim(temp);
        code(temp,line,blockOpition+";"+apiOutVar);
    }else if(title.compare("continue") == 0){
        isBreak = false;
        return "";
    }else if(title.compare("clear") == 0){
        int index1 = find(f,"(",1);
        if(index1 < 0)errMessage(isNotFunction,line,index1,"clear");
        system("CLS");
    }else if(title.compare("system") == 0){
        Pos rpos = block_find_samllK(f,1);
        string command = f.substr(rpos.st,rpos.ed-rpos.st),com2;
        string rt = rsStr(com2,command);
        if(rt != "")errMessage(varNotFound,line,index1,rt);
        return to_string(system(com2.c_str()));
    }else if(title.compare("time") == 0){
        Pos rpos = block_find_samllK(f,1);
        string vname = f.substr(rpos.st,rpos.ed-rpos.st);
        if(vname.find('(') != string::npos)
            vname = code(vname,line,blockOpition,ifStates,true,vname);
        if(!memory.has(vname))errMessage(varNotFound,line,index1,vname);
        time_t t = time(0);
        if(vname == "" || vname == "NULL")return string((const char *)ctime(&t));
        memory.mem[memory.getIndex(vname)].value = (const char *)ctime(&t);
        return string((const char *)ctime(&t));
    }else if(title.compare("getKeyInt") == 0){
        Pos rpos = block_find_samllK(f,1);
        string vname = f.substr(rpos.st,rpos.ed-rpos.st);
        if(vname.find('(') != string::npos)
            vname = code(vname,line,blockOpition,ifStates,true,vname);
        if(!memory.has(vname))errMessage(varNotFound,line,index1,vname);
        int key = getch();
        memory.mem[memory.getIndex(vname)].value = to_string(key);
    }else if(title.compare("getKeyChar") == 0){
        Pos rpos = block_find_samllK(f,1);
        string vname = f.substr(rpos.st,rpos.ed-rpos.st);
        if(vname.find('(') != string::npos)
            vname = code(vname,line,blockOpition,ifStates,true,vname);
        if(!memory.has(vname))errMessage(varNotFound,line,index1,vname);
        char key = getch();
        string c = "";
        c += key;
        memory.mem[memory.getIndex(vname)].value = c;
    }else if(title.compare("getMousePosX") == 0){
        Pos rpos = block_find_samllK(f,1);
        string vname = f.substr(rpos.st,rpos.ed-rpos.st);
        if(vname.find('(') != string::npos)
            vname = code(vname,line,blockOpition,ifStates,true,vname);
        if(!memory.has(vname))errMessage(varNotFound,line,index1,vname);
        POINT pt{0,0};
        GetCursorPos(&pt);
        memory.mem[memory.getIndex(vname)].value = to_string((int)pt.x);
    }else if(title.compare("getMousePosY") == 0){
        Pos rpos = block_find_samllK(f,1);
        string vname = f.substr(rpos.st,rpos.ed-rpos.st);
        if(vname.find('(') != string::npos)
            vname = code(vname,line,blockOpition,ifStates,true,vname);
        if(!memory.has(vname))errMessage(varNotFound,line,index1,vname);
        POINT pt{0,0};
        GetCursorPos(&pt);
        memory.mem[memory.getIndex(vname)].value = to_string((int)pt.y);
    }else if(title.compare("getThisMousePosX") == 0){
        Pos rpos = block_find_samllK(f,1);
        string vname = f.substr(rpos.st,rpos.ed-rpos.st);
        if(vname.find('(') != string::npos)
            vname = code(vname,line,blockOpition,ifStates,true,vname);
        if(!memory.has(vname))errMessage(varNotFound,line,index1,vname);
        HWND win = GetConsoleWindow();
        POINT pt{0,0};
        RECT rt;
        GetCursorPos(&pt);
        GetWindowRect(win,&rt);
        pt.x -= rt.left;
        memory.mem[memory.getIndex(vname)].value = to_string((int)pt.x);
    }else if(title.compare("getThisMousePosY") == 0){
        Pos rpos = block_find_samllK(f,1);
        string vname = f.substr(rpos.st,rpos.ed-rpos.st);
        if(vname.find('(') != string::npos)
            vname = code(vname,line,blockOpition,ifStates,true,vname);
        if(!memory.has(vname))errMessage(varNotFound,line,index1,vname);
        HWND win = GetConsoleWindow();
        POINT pt{0,0};
        RECT rt;
        GetCursorPos(&pt);
        GetWindowRect(win,&rt);
        pt.y -= rt.top;
        memory.mem[memory.getIndex(vname)].value = to_string((int)pt.y);
    }else if(title.compare("CEAPI") == 0){
        return "";
    }else if(title.compare("Sleep") == 0){
        Pos rpos = block_find_samllK(f,1);
        string vname = f.substr(rpos.st,rpos.ed-rpos.st);
        string tstr = "";
        rsStr(tstr,vname);
        Sleep(toInt(tstr));
    }else if(title.compare("while") == 0){
        Pos rpos = block_find_samllK(f,1);
        int swindex = 0,swoldindex;
        string condition = f.substr(rpos.st,rpos.ed-rpos.st);
        vector<string> block;
        string errMsg;
        int result = getConditionResult(condition,errMsg);
        if(result == -1)errMessage(varNotFound,line,index1,errMsg);
        for(int i = 0;i < getCharC(blockOpition,'\n');i++){
            swoldindex = swindex;
            swindex = find(blockOpition,"\n",i+1);
            if(swindex < 0)
                break;
            string temp = blockOpition.substr(swoldindex,swindex - swoldindex);
            Trim(temp);
            block.push_back(temp);
        }
        while(result){
            int dp = maxDepth++;
            decode(block);
            maxDepth--;
            removeBlockVariable(memory,dp);
            result = getConditionResult(condition,errMsg);
            if(result == -1)errMessage(varNotFound,line,index1,errMsg);
        }
    }else if(title.compare("loop") == 0){
        Pos rpos = block_find_samllK(f,1);
        int swindex = 0,swoldindex;
        string countStr = f.substr(rpos.st,rpos.ed-rpos.st);
        string cStr = "";
        string rt = rsStr(cStr,countStr);
        if(rt != "")errMessage(varNotFound,line,index1,rt);
        int count = atoi(cStr.c_str());
        vector<string> block;
        for(int i = 0;i < getCharC(blockOpition,'\n');i++){
            swoldindex = swindex;
            swindex = find(blockOpition,"\n",i+1);
            if(swindex < 0)
                break;
            string temp = blockOpition.substr(swoldindex,swindex - swoldindex);
            Trim(temp);
            block.push_back(temp);
        }
        if(isBreak)
            isBreak = false;
        for(int lpTime = 0;lpTime < count && !isBreak;lpTime++){
            int dp = maxDepth++;
            decode(block);
            maxDepth--;
            removeBlockVariable(memory,dp);
        }
        isBreak = false;
    }else if(title.compare("rloop") == 0){
        int swindex = 0,swoldindex;
        vector<string> block;
        for(int i = 0;i < getCharC(blockOpition,'\n');i++){
            swoldindex = swindex;
            swindex = find(blockOpition,"\n",i+1);
            if(swindex < 0)
                break;
            string temp = blockOpition.substr(swoldindex,swindex - swoldindex);
            Trim(temp);
            block.push_back(temp);
        }
        if(isBreak)
            isBreak = false;
        while(!isBreak){
            int dp = maxDepth++;
            decode(block);
            maxDepth--;
            removeBlockVariable(memory,dp);
        }
        isBreak = false;
    }else if(title.compare("if") == 0){
        Pos rpos = block_find_samllK(f,1);
        int swindex = 0,swoldindex;
        string condition = f.substr(rpos.st,rpos.ed-rpos.st);
        vector<string> block;
        string errMsg;
        int result = getConditionResult(condition,errMsg);
        if(result == -1)errMessage(varNotFound,line,index1,errMsg);
        if(result){
            for(int i = 0;i < getCharC(blockOpition,'\n');i++){
                swoldindex = swindex;
                swindex = find(blockOpition,"\n",i+1);
                if(swindex < 0)
                    break;
                string temp = blockOpition.substr(swoldindex,swindex - swoldindex);
                Trim(temp);
                block.push_back(temp);
            }
            int dp = maxDepth++;
            decode(block);
            maxDepth--;
            removeBlockVariable(memory,dp);
            block.clear();
        }else{
            for(TIfStates::const_iterator it = ifStates->begin();it != ifStates->end();it++){
                block.clear();
                result = getConditionResult(it->condition,errMsg);
                if(result == -1)errMessage(varNotFound,line,index1,errMsg);
                if(result){
                    for(int i = 0;i < getCharC(it->inside,'\n');i++){
                        swoldindex = swindex;
                        swindex = find(it->inside,"\n",i+1);
                        if(swindex < 0)
                            break;
                        string temp = it->inside.substr(swoldindex,swindex - swoldindex);
                        Trim(temp);
                        block.push_back(temp);
                    }
                    int dp = maxDepth++;
                    decode(block);
                    maxDepth--;
                    removeBlockVariable(memory,dp);
                    break;
                }
            }
        }
    }else{
        string varName = title;
        Trim(varName);
        string value = "";
        //cout << f << endl;
        if(findAllSW(f,"+=") != string::npos){
            index1 = findAllSW(f,"+=")+1;
            index2 = f.length();
            if(varName.find(':') != string::npos)
                varName = code(f,line,blockOpition,ifStates,true,f);
            if(!memory.has(varName))errMessage(varNotFound,line,index1,varName);
            string v = f.substr(index1+1,index2 - index1);
            Trim(v);
            string rst = rsStr(value,v);//substr(start,length)
            if(rst != "")errMessage(varNotFound,line,index1,rst);
            int lv = toInt((string &)memory.mem[memory.getIndex(varName)].value);
            int rv = toInt(value);
            errType msg = memory.mem[memory.getIndex(varName)].setValue(to_string(lv+rv));
            errMessage(msg,line,index1,varName);
        }else if(findAllSW(f,"-=") != string::npos){
            index1 = findAllSW(f,"-=")+1;
            index2 = f.length();
            if(varName.find(':') != string::npos)
                varName = code(f,line,blockOpition,ifStates,true,f);
            if(!memory.has(varName))errMessage(varNotFound,line,index1,varName);
            string v = f.substr(index1+1,index2 - index1);
            Trim(v);
            string rst = rsStr(value,v);//substr(start,length)
            if(rst != "")errMessage(varNotFound,line,index1,rst);
            int lv = toInt((string &)memory.mem[memory.getIndex(varName)].value);
            int rv = toInt(value);
            errType msg = memory.mem[memory.getIndex(varName)].setValue(to_string(lv-rv));
            errMessage(msg,line,index1,varName);
        }else if(strInclude('=',f)){
            index1 = f.find_first_of("=") + 1;
            index2 = f.length();
            if(varName.find(':') != string::npos)
                varName = code(f,line,blockOpition,ifStates,true,f);
            if(!memory.has(varName))errMessage(varNotFound,line,index1,varName);
            string v = f.substr(index1,index2 - index1);
            Trim(v);
            string rst = rsStr(value,v);//substr(start,length)
            if(rst != "")errMessage(varNotFound,line,index1,rst);
            errType msg = memory.mem[memory.getIndex(varName)].setValue(value);
            errMessage(msg,line,index1,varName);
        }else{
            if(varName.find(':') != string::npos)
                varName = code(f,line,blockOpition,ifStates,true,f);
            if(!memory.has(varName))errMessage(varNotFound,line,index1,varName);
            return memory.mem[memory.getIndex(varName)].value;
        }
    }
    return "";
}

void beforeRun(vector<string> & in,vector<string> & file){
    bool instring = false;
    bool ls = false;
    bool tsNosee;
    for(int i = 0;i < (int)file.size();i++){
        string tar = "";
        Trim(file[i]);
        if(file[i].compare("'''") == 0){
            tsNosee = !tsNosee;
            continue;
        }
        if(uppercase(file[i]).compare("NOTESSTART;") == 0){
            tsNosee = true;
            continue;
        }else if(uppercase(file[i]).compare("NOTESEND;") == 0){
            tsNosee = false;
            continue;
        }
        if(tsNosee){
            continue;
        }
        for(int u = 0;u < (int)file[i].length();u++){
            if(file[i][u] == '\"' && file[i][u-1] != '\\')
                instring = !instring;
            if(file[i][u] == '+' && !instring){
                int off = 1;
                while(true){
                    if(file[i][u+off] == '+'){
                        tar += "+= 1";
                        u += off;
                        ls = true;
                        break;
                    }else if(isspace(file[i][u+off])){
                        off++;
                    }else{
                        break;
                    }
                }
            }else if(file[i][u] == '-' && !instring){
                int off = 1;
                while(true){
                    if(file[i][u+off] == '-'){
                        tar += "-= 1";
                        u += off;
                        ls = true;
                        break;
                    }else if(isspace(file[i][u+off])){
                        off++;
                    }else{
                        break;
                    }
                }
            }
            if(ls){
                ls = false;
                continue;
            }
            if(file[i][u] == '}' && !instring){
                tar += file[i][u];
                if(tar.compare("") != 0)
                    in.push_back(tar);
                tar = "";
            }else{
                if(file[i][u] == '/' && file[i][u+1] == '/')
                    break;
                tar += file[i][u];
            }
        }
        Trim(tar);
        if(tar.compare("") != 0)
            in.push_back(tar);
    }
    if(tsNosee){
        warPrint("\nwaring:A,oh,Your notes have no ending!!!\nChecking your notes now!!!\n"
                 "警告:A，Oh,你的注释没有结束!!!\n现在检查你的注释吧!!!(如果你对结果有疑惑的话)\n");
    }
}

void decode(vector<string> & file){
    for(unsigned int i = 0;i < file.size() && !isBreak;i++){
        if(Trim(file[i]).compare("}") == 0){
            continue;
        }
        int oldIndex;
        int index = 0;
        int u = 0;
        bool block = false;
        Pos rin = block_find_samllK(file[i],1);
        string titles = getTitle(file[i]);
        for(auto i : blocks){
            if(titles.compare(i) == 0){
                block = true;
                break;
            }
        }
        if(block){
            string temp = file[i].substr(0,rin.ed+1);
            string lineTmp = temp;
            string nallFile = "",lnallFile;
            PTIfStates ifStates = NULL;
            int ui;
            int ln = 0;
            for(unsigned int nu = i;nu < file.size();nu++){
                nallFile += file[nu];
                lnallFile += file[nu]+'\n';
            }
            Pos ps2 = block_find_bigK(nallFile,1),ps3 = block_find_bigK(lnallFile,1);
            lineTmp += "{";
            lineTmp += nallFile.substr(ps2.st,ps2.ed-ps2.st);
            lineTmp += "}";
            if(titles.compare("if") == 0){
                static TIfStates states;
                ifStates = &states;
                ifStates->clear();
                string elses = lnallFile.substr(ps3.ed+1,lnallFile.length() - ps3.ed);
                Trim(elses);
                if(elses.compare("") != 0){
                    while(true){
                        Trim(elses);
                        string iftitle = getTitle(elses);
                        if(iftitle.compare("else") == 0){
                            Pos ecwPos = block_find_samllK(elses,1);
                            int index1 = elses.find_first_of("( +");
                            string ecw = elses.substr(index1,ecwPos.st - index1-1);
                            Trim(ecw);
                            if(ecw.compare("if") == 0){
                                rin = block_find_samllK(elses,1);
                                lineTmp += elses.substr(0,rin.ed+1);
                                ps2 = block_find_bigK(elses,1);
                                lineTmp += "{";
                                lineTmp += getBlockData(1,elses).inside;
                                lineTmp += "}";
                                Pos rpos = block_find_samllK(elses,1);
                                string condition = elses.substr(rpos.st,rpos.ed-rpos.st);
                                string inside = getBlockData(1,elses).inside;
                                inside += "\n";
                                (*ifStates).push_back(PSStateID(condition,inside));
                                elses = elses.substr(ps2.ed+1,elses.length()-ps2.ed);
                            }else{
                                rin = block_find_samllK(elses,1);
                                lineTmp += elses.substr(0,rin.ed+1);
                                ps2 = block_find_bigK(elses,1);
                                lineTmp += "{";
                                lineTmp += getBlockData(1,elses).inside;
                                lineTmp += "}";
                                string condition = "\"$sys$#else\"";
                                string inside = getBlockData(1,elses).inside;
                                inside += "\n";
                                (*ifStates).push_back(PSStateID(condition,inside));
                                break;
                            }
                        }else{
                            break;
                        }
                    }
                }
            }
            ui = (int)lineTmp.length();
            for(unsigned int nu = i;nu < file.size() && ui > 0;nu++){
                int lsize = file[nu].length();
                if(ui < lsize){
                    break;
                }
                ui -= lsize;
                ln++;
            }
            code(temp,i+ln,getBlockData(1,lnallFile).inside,ifStates);
            if(ceGetCharC(file[i + ln -1],'}') >= 1){
                i += ln-1;
            }else{
                i += ln;
            }
            //cout << "now line:" << i << ":" << file[i] << endl;
            if(i >= file.size())
                return;//处理i超出界限的问题，如不处理，结束运行时将会卡很久
        }
        if(Trim(file[i])[0] != '#')
            for(u = 0;u < getCharC(file[i],';');u++){
                oldIndex = index;
                index = find(file[i],";",u+1);
                if(index == -1)
                    break;
                if(index == -2)
                    break;
                code(file[i].substr(oldIndex,index - oldIndex-1),i+1,"");
            }
        else
            code(file[i],i+1,"");
    }
}

void loadSystem(){
    string path = "";
    fstream stream;
    const int filesSize = 1;
    string files[filesSize]{"sysVars.rb"};
    vector<string> tempFile,filen;
    path += _pgmptr;
    int last = path.find_last_of('\\');
    path = path.substr(0,last+1);
    path += "system";
    for(int i = 0;i < filesSize;i++){
        string file = path;
        file += "\\";
        file += files[i];
        stream.open(file,ios::in);
        if(!stream.is_open()){
            errPrint("未找到"+file+".\n可能已丢失！！！丢失后将会影响内容！！！\n");
            Sleep(2000);
            system("CLS");
            return;
        }
        while (!stream.eof())
        {
            string strtemp;
            strtemp.resize(1024);
            stream.getline((char *)strtemp.c_str(),1024);
            Trim(strtemp);
            strtemp.resize(strlen(strtemp.c_str()) + 1);//success
            tempFile.push_back(strtemp);
        }
        filen.insert(filen.end(), tempFile.begin(), tempFile.end());
        int dp = maxDepth++;
        decode(filen);//解码
        maxDepth--;
        removeBlockVariable(memory,dp);
        tempFile.clear();
        filen.clear();
    }
}

int toInt(std::string & str){
    return atoi(str.c_str());
}

string getTitle(string line,string gv){
    Trim(line);
    int index1 = line.find_first_of(gv);
    std::string title = line.substr(0,index1 - 0);
    return title;
}

BlockData getBlockData(int type,string line){
    Pos ps;
    BlockData data;
    data.init();
    switch(type){
    case 0:
        ps = block_find_samllK(line,1);
        break;
    case 1:
        ps = block_find_bigK(line,1);
        break;
    default:
        BlockData * dtNull =  NULL;
        return *dtNull;
    }
    data.head = line.substr(0,ps.st-1);
    data.inside = line.substr(ps.st,ps.ed-ps.st);
    data.operators[0] = (type == 0)?'(':'{';
    data.operators[1] = (type == 0)?')':'}';
    Trim(data.head);
    Trim(data.inside);
    return data;
}

template<class T> void reverse(T & t){
    T temp;
    for(typename T::reverse_iterator it = t.rbegin();it != t.rend();it++){
        temp.insert(make_pair(it->first,it->second));
    }
    t.clear();
    t = T(temp);
}

void removeBlockVariable(CeMemory & mem,int depth){
    CeMemory temp;
    temp.mem.clear();
    for(int i = 0;i < (int)mem.mem.size();i++){
        if(mem.mem[i].depth != depth)
            temp.mem.push_back(mem.mem[i]);
        //else
           // cout << "remove a variable which named " << mem.mem[i].name << ",the value is" << mem.mem[i].value << endl;
    }
    mem.mem.clear();
    for(int i = 0;i < (int)temp.mem.size();i++)
        mem.mem.push_back(temp.mem[i]);
}

string::size_type findAllSW(string tar,string cmp,int * stg){
    return tar.find(cmp);
}

template<class T> vector<T> sortVector(vector<T> read){
    sort(read.begin(),read.end(),[&](int x,int y)->bool{return x>y;});
    return read;
}

int findBiggest(vector<int> bg){
    return sortVector(bg)[0];
}

Pos findInsidestC(string tar){
    vector<int> depths;
    int ndep = 0;
    bool instr = false;
    for(int i = 0;i < (int)tar.length();i++){
        if(tar[i] == '\"')instr = !instr;
        else if(tar[i] == '\\' && instr){i++;continue;}
        if(tar[i] == '(')depths.push_back(++ndep);
        else if(tar[i] == ')')ndep--;
    }
    int i = findBiggest(depths);
    vector<int>::iterator it = find(depths.begin(),depths.end(),i);
    i = it - depths.begin();
    return block_find_samllK(tar,i+1);
}

string replaceStr(string tar,string token,string rp){
    string temp;
    int index = tar.find(token);
    temp += tar.substr(0,index) + rp + tar.substr(index+token.length(),tar.length()-(index+token.length()));
    return temp;
}

string operator+(string a,string b){string c = a;c += b;return c;}
string operator+(const char * a,string b){string c = a;c += b;return c;}
string operator+(string b,const char * a){string c = b;c += a;return c;}

string uppercase(string tar){
    string rt;
    for(auto i : tar){
        rt += toupper(i);
    }
    return rt;
}

errType use_pragma(vector<string> args){
    int size = (int)args.size();
    if(size < 1)return argNotEnough;
    for(int i = 0;i < (int)args.size();i++){Trim(args[i]);}
    if(args[0].compare("waring") == 0){
        if(size < 2)return argNotEnough;
        warings.push_back(atoi(args[1].c_str()));
    }else if(args[0].compare("author") == 0){
        if(size < 2)return argNotEnough;
        string all,all2;
        for(vector<string>::iterator it = ++args.begin();it < args.end();it++)
            all += args[it-args.begin()];
        string rs = rsStr(all2,all,-999);
        if(rs !=  "")errMessage(varNotFound,0,0,rs);
        info.authorName = all2;
        string au = "author__";
        if(memory.has(au))
            memory.mem[memory.getIndex(au)].setValue(info.authorName,true);
    }else if(args[0].compare("inputColor") == 0){
        if(size < 2)return argNotEnough;
        string all,all2;
        for(vector<string>::iterator it = ++args.begin();it < args.end();it++)
            all += args[it-args.begin()];
        string rs = rsStr(all2,all,-999);
        if(rs !=  "")errMessage(varNotFound,0,0,rs);
        if(all2.compare("original") == 0)SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE),consoleinfoin.wAttributes);
        int color = toInt(all2);
        WORD inback = consoleinfoin.wAttributes & ~white;
        SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE),color | inback);
    }else if(args[0].compare("outputColor") == 0){
        if(size < 2)return argNotEnough;
        string all,all2;
        for(vector<string>::iterator it = ++args.begin();it < args.end();it++)
            all += args[it-args.begin()];
        string rs = rsStr(all2,all,-999);
        if(rs !=  "")errMessage(varNotFound,0,0,rs);
        if(all2.compare("original") == 0)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),consoleinfoo.wAttributes);
        int color = toInt(all2);
        WORD inback = consoleinfoo.wAttributes & ~white;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color | inback);
    }else if(args[0].compare("errorColor") == 0){
        if(size < 2)return argNotEnough;
        string all,all2;
        for(vector<string>::iterator it = ++args.begin();it < args.end();it++)
            all += args[it-args.begin()];
        string rs = rsStr(all2,all,-999);
        if(rs !=  "")errMessage(varNotFound,0,0,rs);
        if(all2.compare("original") == 0)SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE),consoleinfoe.wAttributes);
        int color = toInt(all2);
        WORD inback = consoleinfoe.wAttributes & ~white;
        SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE),color | inback);
    }else if(args[0].compare("CheckApi") == 0){
        if(size < 2)return argNotEnough;
        if(uppercase(args[1]).compare("FALSE") == 0) checkApi = false;
        else if(uppercase(args[1]).compare("TRUE") == 0)checkApi = true;
        else errMessage(argNotGood,0,0,"\nPosition:"+args[1]+" in #pragma!!!!! 只有true/false两个值！！！");
    }else errMessage(varNotFound,0,0,"\nPosition:"+args[1]+" in 只有,outputColor,inputColor,CheckApi,errorColor,author,waring");
    return none;
}

void * timeable(void * arg){
    info.runTime = 0;
    string var = "runtime__";
    while(true){
        info.runTime+=10;
        memory.mem[memory.getIndex(var)].setValue(to_string(info.runTime),true);
        pthread_testcancel();
        Sleep(8);
    }
    return NULL;
}

BOOL loginCtrl(DWORD flags){
    switch(flags){
    case CTRL_CLOSE_EVENT:

        return TRUE;
    case CTRL_SHUTDOWN_EVENT:

        return TRUE;
    }
    return FALSE;
}

bool warsHas(unsigned int v){
    vector<unsigned int>::iterator it = find(warings.begin(),warings.end(),v);
    if((it-warings.begin()) == (warings.end()-warings.begin()))return false;
    return true;
}

int colorfulPrint(const string message,WORD color){
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    WORD inback = info.wAttributes & ~white;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color | inback);
    int rt = printf("%s",message.c_str());
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),info.wAttributes);
    return rt;
}

int errPrint(const string message){
    return colorfulPrint(message,red);
}

int warPrint(const string message){
    return colorfulPrint(message,blue);
}

errType varRule(string varName){
    Trim(varName);
    string nc = "+-*/&^!@~`=?><.,'\"| %#(){}[]:";
    if(isdigit(varName[0]))return varNotGood;
    for(auto i : varName){
        if(i == '\0')continue;
        if(nc.find(i) != string::npos)return varNotGood;
    }
    return none;
}

errType apiUse(int key,string funcName,vector<string> funcArgs,string & return_){
    switch(key){
    case 0:
        return_ = funcName;
        return noAcess;
    case 1://系统API之一
        if(funcName.compare("to_real") == 0){
            string give = "";
            string err;
            for(auto i : funcArgs){
                string ri = "";
                if((err = rsStr(ri,i,0)) != ""){return_ = err;return varNotFound;}
                give += ri;
            }
            return_ = give;
            return none;
        }else {return_ = funcName;return varNotFound;}
    default:
        return_ = "\"NO SUCH API TOKEN\"";
        return varNotFound;
    }
}
