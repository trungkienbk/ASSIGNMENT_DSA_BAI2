#include "SymbolTable.h"
using namespace std;
void SymbolTable::run(string filename)
{
    int cur_level = 0;
    ifstream f(filename);
    while (!f.eof()){
        string ins,name,var;
        getline(f, ins);
        ins.erase( remove(ins.begin(), ins.end(), '\r'), ins.end() );
        if(regex_match(ins,ins_vari)){
            cout<<"Insert variable oke"<<endl;
        }
        else if (regex_match(ins,ins_func)){
            cout<<"Insert func oke "<<endl;
        }
        else if (regex_match(ins, ass_val)){
            cout<<"Assign value oke"<<endl;
        }
        else if(regex_match(ins, ass_vari)){

            cout<<"Assign variable oke"<<endl;
        }
        else if(regex_match(ins,ass_func)){
            cout<<"Assign func oke "<<endl;
        }
        else if(regex_match(ins,look_up)){
            int space =(int) ins.find(' ');
            name=ins.substr(space+1);
            lookup(name,cur_level,ins);
        }
        else if(ins == "PRINT"){
            preOrder();
            cout<<"Print func oke"<<endl;
        }
        else if(ins == "BEGIN"){
            cur_level++;
            cout<<"Begin func oke"<<endl;
        }
        else if(ins =="END"){
           if(cur_level == 0){
                throw UnknownBlock();
            }
            while(dList->top().scope == cur_level){
                this->removeTree(dList->top());
                dList->pop();
            }
            cur_level--;
            cout<<"End fucn oke"<<endl;
        }
        else throw InvalidInstruction(ins);
    }
    f.close();
}
void SymbolTable::preOrderRec(Node *cur,string &s) {
    if(cur == nullptr){
        return;
    }
    s+=cur->val.name + "//" + to_string(cur->val.scope)+ " ";
    if(cur->left){
        inOrderRec(cur->left,s);
    }
    if(cur->right){
        inOrderRec(cur->right,s);
    }
}
void SymbolTable::preOrder() {
    string s = "";
    preOrderRec(root,s);
  /*  int length = (int) s.length();
    if(length == 0) cout<<endl;
    s.pop_back(); */
    cout<<s<<endl;
}
void SymbolTable::inOrderRec(Node *&cur ,string &s) {
    if(cur == nullptr){
        return;
    }
    if(cur->left){
        inOrderRec(cur->left,s);
    }
    s+=cur->val.name + "//" + to_string(cur->val.scope)+ " ";
    if(cur->right){
        inOrderRec(cur->right,s);
    }
}
void SymbolTable::inOrder() {
    string s;
    inOrderRec(root,s);
    int length = (int) s.length();
    if(length == 0) cout<<endl;
    s.pop_back();
    cout<<s;
}
void SymbolTable::rightRotate(Node *&cur) {
    Node *l = cur->left;      //c->t , z->cur
    Node *t = l->right;
    Node *p = cur->parent;
    if(p!= nullptr){
        if(p->left ==cur) {
            p->left=l;
        }
        else {
            p->right = l;
        }
    }
    l->parent = p;
    l->right = cur;
    cur->parent =l;
    cur->left = t;
    if( t != nullptr){
        t->parent = cur;
    }
}
void SymbolTable::leftRotate(Node *&cur) {
    Node *r = cur->right;
    Node *t = r->left;
    Node *p = cur->parent;
    if(p != nullptr){
        if(p->left == cur){
            p->left = r;
        }
        else {
            p->right = r;
        }
        r->parent = p;
        r->left = cur;
        cur->parent = r;
        cur->right = t;
        if(t != nullptr){
            t->parent = cur;
        }
    }
}
void SymbolTable::splay(Node *&cur) {
    if ( cur == nullptr)
        return;
    while (true)
    {
        Node *pare = cur->parent;
        if (pare == nullptr)
        {
            // cur is root
            break;
        }
        Node *gPare = pare->parent;
        if (gPare == nullptr && pare->left == cur)
        {
            // zig
            rightRotate(pare);
        }
        else if (gPare == nullptr && pare->right == cur)
        {
            // zag
            leftRotate(pare);
        }
        else if (gPare->left == pare && pare->left == cur)
        {
            // zig-zig
            rightRotate(gPare);
            rightRotate(pare);
        }
        else if (gPare->right == pare && pare->right == cur)
        {
            // zag-zag
            leftRotate(gPare);
            leftRotate(pare);
        }
        else if (gPare->left == pare && pare->right == cur)
        {
            // zig-zag
            leftRotate(pare);
            rightRotate(gPare);
        }
        else if(gPare->right == pare && pare -> left ==cur)
        {
            // zag-zig
            rightRotate(pare);
            leftRotate(gPare);
        }
    }
    root = cur;
}
Node *SymbolTable::searchLevell(string name, int level) {
    Symbol *e= new Symbol(name,"null",level);
    if (root == nullptr){
        delete e;
        return nullptr;
    }
    Node *cur=root;
    while (true)
    {
        if (cur->val == *e) break;
        else if (cur->val > *e){
            if (cur->left == nullptr)
                break;
            else {
                cur = cur->left;
            }
        }
        else{
            if (cur->right == nullptr)
                break;
            else {
                cur = cur->right;
            }
        }
    }
    if (cur->val == *e){
        delete e;
        return cur;
    }
    else{
        delete e;
        return nullptr;
    }
}
void SymbolTable::removeTree(Symbol element) {
    Node *temp = searchLevell(element.name,element.scope);
    if (temp == nullptr)
        return ;
    splay(temp);
    Node *t = temp->left;
    if(temp->right == nullptr){
        root=temp->left;
    }
    if (t == nullptr)
    {
        root = temp->right;
        root->parent = nullptr;
    }
    else
    {
        while (t->right)
            t = t->right;
        if (temp->right != nullptr)
        {
            t->right = temp->right;
            temp->right->parent=t;
        }
        root = temp->left;
        root->parent = nullptr;
    }
    delete(temp);
    return;
}
Symbol SymbolTable::isContains(string name, int level) {
    Symbol e("null","null",-1);
    Node *temp = new Node(e);
    if(root == nullptr) return e;
        for(int i = level; i >= 0 ;--i){
            temp = searchLevell(name,i);
            e.name = temp->val.name;
            e.type = temp->val.type;
            e.scope = temp ->val.scope;
            if(e.name != "null") break;
    }
        if(temp->val.name == "null") delete temp;
        return e;
}
void SymbolTable::lookup(string name, int level, string ins) {
    if(root == nullptr) throw Undeclared(ins);
    Symbol e("null","null",-1);
    Node *temp = new Node(e);
    for(int i = level; i >= 0 ;--i){
        temp = searchLevell(name,i);
        e.name = temp->val.name;
        e.type = temp->val.type;
        e.scope = temp ->val.scope;
        if(e.name != "null") break;
    }
    if(temp->val.name == "null") {
        delete temp;
        throw Undeclared(ins);
    }
    splay(temp);
    cout<<temp->val.scope<<endl;
}





































/*Node *SymbolTable::findNode(string name,int level) {
    Symbol *e= new Symbol(name,"null",level);
    if (root == nullptr){
        delete e;
        return nullptr;
    }
    Node *cur=root;
    while (true)
    {
        if (cur->val == *e) break;
        else if (cur->val > *e){
            if (cur->left == nullptr)
                break;
            else {
                cur = cur->left;
            }
        }
        else{
            if (cur->right == nullptr)
                break;
            else {
                cur = cur->right;
            }
        }
    }
    if (cur->val == *e){
        delete e;
        return cur;
    }
    else{
        delete e;
        return nullptr;
    }
}
*/




























/*#include "SymbolTable.h"
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
*/
