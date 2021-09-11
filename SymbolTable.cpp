#include "SymbolTable.h"
using namespace std;
string isValidIns(string instruction){
    if(instruction=="BEGIN" || instruction=="END" || instruction == "PRINT" ||instruction == "RPRINT") return instruction;
    if(!regex_match(instruction, in) && !regex_match(instruction, as) && !regex_match(instruction, lk)){
        throw InvalidInstruction(instruction);
    }
    if(regex_match(instruction, in)) return "INSERT";
    if(regex_match(instruction, as))return "ASSIGN";
    if(regex_match(instruction, lk)) return "LOOKUP";
    return "";
}
int convertInt(string method ){
    if(method == "INSERT") return 1;
    if(method == "LOOKUP") return 2;
    if(method == "ASSIGN") return 3;
    if(method == "PRINT") return 4;
    if(method == "RPRINT") return 5;
    if(method == "BEGIN") return 6;
    return 7;
}

void SymbolTable::run(string filename)
{
   // SymbolTable table;
    int level = 0;
    string ins;
    ifstream input;
    input.open(filename);
    if(input.is_open()) {
        while (std::getline(input, ins)) {
            string method,name,var; // var  = type or value
            method = isValidIns(ins);
            int numberMethod = convertInt(method);
            //Insert: 1 Lookup:2 Assign:3 Print:4 RPrint:5 Begin:6 End:7
            switch (numberMethod) {
                case 1 : { // INSERT
                    int index[2]={0,0};
                    int j = 0;
                    for(int i=0;i<(int)ins.size();++i){
                        if(j==2) break;
                        if(ins[i]==' '){
                            index[j]=i;
                            j++;
                        }
                    }
                    name = ins.substr(index[0]+1,index[1]-index[0]-1);
                    var  = ins.substr(index[1]+1);
                //     Symbol check =  table.search(name,level);
                    Symbol check =  this->search(name,level);
                    if(check.scope ==-1 || (check.scope !=-1 && check.scope < level)){
                        Symbol symbol(name,var,level);
                      //  table.push(symbol);
                         push(symbol);
                        cout<<"success"<<endl;
                    }
                    else {
                       // table.clear();
                        this->clear();
                        throw Redeclared(ins);
                    }
                    break;
                }
                case 2 : {
                    int space =(int) ins.find(' ');
                    name=ins.substr(space+1);
                    //Symbol check = table.search(name,level);
                    Symbol check = this->search(name,level);
                    if(check.scope == -1){
                        this->clear();
                        throw Undeclared(ins);
                    }
                    else {
                        cout<<check.scope<<endl;
                    }
                    break;
                }
                case 3 : {
                    int index[2]={0,0};
                    int j = 0;
                    for(int i=0;i<(int)ins.size();++i){
                        if(j==2) break;
                        if(ins[i]==' '){
                            index[j]=i;
                            j++;
                        }
                    }
                    name = ins.substr(index[0]+1,index[1]-index[0]-1);
                    var  = ins.substr(index[1]+1);
                    assign(name,var,level,ins);
                    break;
                }
                case 4 : {
                    this->print();
                    break;
                }
                case 5 : {
                    this->rprint();
                    break;
                }
                case 6 : {
                    level++;
                    break;
                }
                default :
                    if(level == 0) {
                        //table.clear();
                        this->clear();
                        throw UnknownBlock();
                    }
                    //table.pop_scope(level);
                    this->pop_scope(level);
                    level--;
                    break;
            }
        }
        if(level > 0) {
           // table.clear();
            this->clear();
            throw UnclosedBlock(level);
        }
        input.close();
    }
    else {
        cout<<filename<<" is not exist";
    }
}
void SymbolTable::assign(string name, string var, int level, string ins) {
    if(regex_match(var,re_num)){
        Symbol check = search(name,level);
        if(check.scope==-1) throw Undeclared(ins);
        if(check.type=="string") throw TypeMismatch(ins);
        cout<<"success"<<endl;
        return;
    }
    if(regex_match(var,re_str) && !regex_match(var,re_num)){
        Symbol check = search(name,level);
        if(check.scope==-1) throw Undeclared(ins);
        if(check.type=="number") throw TypeMismatch(ins);
        cout<<"success"<<endl;
        return;
    }
    if(!regex_match(var,re_str) && !regex_match(var,re_num)){
        Symbol check_x = search(var,level);
        Symbol check_y = search(name,level);
        if(check_x.scope==-1 || check_y.scope==-1) throw Undeclared(ins);
        if(check_x.type != check_y.type ) throw TypeMismatch(ins);
        cout<<"success"<<endl;
        return;
    }
}