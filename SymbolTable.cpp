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
{   SymbolTable table;
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
                    Symbol check = search(name,level);
                    if(check.scope ==-1 || (check.scope !=-1 && check.scope < level)){
                        Symbol symbol(name,var,level);
                        table.push(symbol);
                        cout<<"success"<<endl;
                    }
                    else {
                        table.clear();
                        throw Redeclared(ins);
                    }
                    break;
                }
                case 2 : {
                    int space =(int) ins.find(' ');
                    name=ins.substr(space+1);
                    Symbol check = search(name,level);
                    if(check.scope == -1) throw Undeclared(ins);
                    else {
                        cout<<check.scope<<endl;
                    }
                    break;
                }
                case 3 : {
                break;
                }
                case 4 : {
                    break;
                }
                case 5 : {
                    break;
                }
                case 6 : {
                    table.pop_scope(level);
                    level++;
                    break;
                }
                default :
                    if(level == 0) throw UnknownBlock();
                    table.pop_scope(level);
                    level--;
                    break;
            }
        }
        if(level > 0) throw UnclosedBlock(level);
        input.close();
    }
    else {
        cout<<filename<<" is not exist";
    }
}
/*bool SymbolTable::isContains(Symbol symbol, vector<pair<string, int>> key) { //Neu da ton tai -> True
    int length = key.size();
    for(int i = 0 ; i < length ; ++i){
        if(symbol.name == key[i].first){
            return true;
        }
    }
    return false;
}

void SymbolTable::print(vector<vector<Symbol>> &list, int level) {
    vector<pair<string,int>> key; // pair<name,
    for(int i = level ; i >=0 ;--i){
        for(int j = list.at(i).size()-1;j>=0;--j){
            if(isContains(list.at(i).at(j),key)){
                continue; // If contains => continue
            }
            else{
                pair<int,int> temp=search(list,list.at(i).at(j).name,level);
                if(temp.first == -1 ) continue;
                else {
                    pair<string,int> temp_push = make_pair(list.at(i).at(j).name,i);
                    key.push_back(temp_push);
                }
            }
        }
    }
    if(key.size()==0){
        return;
    }
    for(int i = key.size()-1; i>0;i--){
        cout<<key[i].first<<"//"<<key[i].second<<" ";
    }
    cout<<key[0].first<<"//"<<key[0].second<<endl;
}

void SymbolTable::rprint(vector<vector<Symbol>> &list, int level) {
    vector<pair<string,int>> key; // pair<name,
    for(int i = level ; i >=0 ;--i){
        for(int j = list.at(i).size()-1;j>=0;--j){
            if(isContains(list.at(i).at(j),key)){
                continue; // If contains => continue
            }
            else{
                pair<int,int> temp=search(list,list.at(i).at(j).name,level);
                if(temp.first == -1 ) continue;
                else {
                    pair<string,int> temp_push = make_pair(list.at(i).at(j).name,i);
                    key.push_back(temp_push);
                }
            }
        }
    }
    if(key.size()==0){
        return;
    }
    for(int i = 0; i < (int) key.size()-1;i++){
        cout<<key[i].first<<"//"<<key[i].second<<" ";
    }
    int end = key.size()-1;
    cout<<key[end].first<<"//"<<key[end].second<<endl;
}
pair<int,string> SymbolTable::search_str(vector<vector<Symbol>> &list, string name,int level) {
    for(int i = level;i >=0; --i){
        for(int j = list.at(i).size()-1;j>=0;--j){
            if(list.at(i).at(j).name == name ){
                pair<int,string> temp = make_pair(i,list.at(i).at(j).type); // tim thay
                return temp;
            }
        }
    }
    return {-1,""}; // ko tim thay
}
void SymbolTable::assign(vector<vector<Symbol>> &list,string name,string var, int level,string ins) {
    if(regex_match(var,re_num)){ // assign x 19
        pair<int,string> check = search_str(list,name,level);
        if(check.first==-1) throw Undeclared(ins);
        if(check.second=="string") throw TypeMismatch(ins);
        cout<<"success"<<endl;
        return;
    }
    if(regex_match(var,re_str) && !regex_match(var,re_num)){
        pair<int,string> check = search_str(list,name,level);
        if(check.first==-1) throw Undeclared(ins);
        if(check.second=="number") throw TypeMismatch(ins);
        cout<<"success"<<endl;
        return;}
    if(!regex_match(var,re_str) && !regex_match(var,re_num)){
        pair<int,string> check_x = search_str(list,var,level);
        pair<int,string> check_y = search_str(list,name,level);
        if(check_x.first==-1 || check_y.first==-1) throw Undeclared(ins);
        if(check_x.second != check_y.second ) throw TypeMismatch(ins);
        cout<<"success"<<endl;
        return;
    }
}
*/