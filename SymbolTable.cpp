#include "SymbolTable.h"
using namespace std;
void SymbolTable::run(string filename)
{
    int cur_level = 0;
    ifstream f(filename);
    while (!f.eof()){
        string ins,name,var,isStatic,decode;
        getline(f, ins);
        ins.erase( remove(ins.begin(), ins.end(), '\r'), ins.end() );
        if(regex_match(ins,ins_vari)) {     ///// HAM INSERT BIEN
            int count = 0;
            int index[3]={0,0,0};
            int j = 0;
            for(int i=0;i<(int)ins.size();++i){
                if(j==3) break;
                if(ins[i]==' '){
                    index[j]=i;
                    j++;
                }
            }
            name = ins.substr(index[0]+1,index[1]-index[0]-1);
            var  = ins.substr(index[1]+1,index[2]-index[1]-1);
            isStatic = ins.substr(index[2]+1);
            Symbol e(name,var,cur_level);
            if(isStatic == "true"){
                e.scope = 0;
            } else {
                dList.push(e);
                if(dList.head->val.scope == 0){
                    dList.pop();
                }
            }
            Node *temp = searchLevell(e.name,e.scope);
            if(temp== nullptr){
                insertNode(e,count);
            }
            else {
                throw Redeclared(ins);
            }
        }
        else if (regex_match(ins,ins_func)){        ///// HAM INSERT FUNCTION
            int count = 0;
            int index[3]={0,0,0};
            int j = 0;
            for(int i=0;i<(int)ins.size();++i){
                if(j==3) break;
                if(ins[i]==' '){
                    index[j]=i;
                    j++;
                }
            }
            name = ins.substr(index[0]+1,index[1]-index[0]-1);
            decode  = ins.substr(index[1]+1,index[2]-index[1]-1);
            isStatic = ins.substr(index[2]+1);
            decode.erase( remove(decode.begin(), decode.end(), '-'), decode.end() );
            decode.erase( remove(decode.begin(), decode.end(), '>'), decode.end() );
            decode.erase( remove(decode.begin(), decode.end(), '('), decode.end() );
            decode.erase( remove(decode.begin(), decode.end(), '('), decode.end() );
            decode.erase( remove(decode.begin(), decode.end(), ')'), decode.end() );
            decode.erase( remove(decode.begin(), decode.end(), ','), decode.end() );
            while(true){
                int n = decode.find("number");
                if(n == -1) break;
                if(decode.substr(n,6)=="number"){
                    decode.replace(n,6,"1");
                }
            }
            while(true){
                int n = decode.find("string");
                if(n == -1) break;
                if(decode.substr(n,6)=="string"){
                    decode.replace(n,6,"0");
                }
            }
            if(decode[decode.length()-1] == '1'){
                var = "number";
            }
            else var = "string";
            Symbol e(name,var,cur_level);
            if(isStatic == "true"){
                e.scope = 0;
            } else {
                if(cur_level > 0){
                    throw InvalidInstruction(ins);
                }
                dList.push(e);
                if(dList.head->val.scope == 0){
                    dList.pop();
                }
            }
            Node *temp = searchLevell(e.name,e.scope);
            if(temp== nullptr){
                e.decodes = decode;
                insertNode(e,count);
            }
            else {
                throw Redeclared(ins);
            }
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
        }
        else if(ins == "BEGIN"){
            cur_level++;
        }
        else if(ins =="END"){
           if(cur_level == 0){
                throw UnknownBlock();
           }
           while(dList.head && dList.head->val.scope == cur_level){
               removeTree(dList.head->val);
               dList.pop();
           }
            cur_level--;
        }
        else throw InvalidInstruction(ins);
    }
    if(cur_level > 0 ){
        throw UnclosedBlock(cur_level);
    }
    f.close();
}
void SymbolTable::preOrderRec(Node *cur,string &s) {
    if(cur == nullptr){
        return;
    }
    s+=cur->val.name + "//" + to_string(cur->val.scope)+ " ";
    if(cur->left){
        preOrderRec(cur->left,s);
    }
    if(cur->right){
        preOrderRec(cur->right,s);
    }
}
void SymbolTable::preOrder() {
    string s = "";
    preOrderRec(root,s);
    if(s.length() ==0 ) {
        cout<<endl;
        return;
    } else {
        s.pop_back();
    }
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
void SymbolTable::rightRotate(Node *&z) {
    Node *l = z->left;
    Node *c = l->right;
    Node *p = z->parent;
    if (p != nullptr)
    {
        if (p->left == z)
            p->left = l;
        else
            p->right = l;
    }
    l->parent = p;
    l->right = z;
    z->parent = l;
    z->left = c;
    if (c != nullptr)
        c->parent = z;
}
void SymbolTable::leftRotate(Node *&z) {
    Node *r = z->right;
    Node *c = r->left;
    Node *p = z->parent;
    if (p != nullptr)
    {
        if (p->left == z)
            p->left = r;
        else
            p->right = r;
    }
    r->parent = p;
    r->left = z;
    z->parent = r;
    z->right = c;
    if (c != nullptr)
        c->parent = z;
}
void SymbolTable::splay(Node *&z) {
    if (z == nullptr)
        return;
    while (true)
    {
        Node *par = z->parent;
        if (par == nullptr)
        {
            // z is the root
            break;
        }
        Node *gPar = par->parent;
        if (gPar == nullptr && par->left == z)
        {
            // zig
            rightRotate(par);
        }
        else if (gPar == nullptr && par->right == z)
        {
            // zag
            leftRotate(par);
        }
        else if (gPar->left == par && par->left == z)
        {
            // zig-zig
            rightRotate(gPar);
            rightRotate(par);
        }
        else if (gPar->right == par && par->right == z)
        {
            // zag-zag
            leftRotate(gPar);
            leftRotate(par);
        }
        else if (gPar->left == par && par->right == z)
        {
            // zig-zag
            leftRotate(par);
            rightRotate(gPar);
        }
        else if(gPar->right == par && par -> left ==z)
        {
            // zag-zig
            rightRotate(par);
            leftRotate(gPar);
        }
    }
    root = z;
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
        Node *z = searchLevell(element.name,element.scope);
        if (z == nullptr)
            return ;
        splay(z);
        Node *t = z->left;
        if (t == nullptr)
        {
            root = z->right;
            if(root != nullptr) root->parent = nullptr;
        }
        else
        {
            while (t->right)
                t = t->right;
            if (z->right != nullptr)
            {
                t->right = z->right;
                z->right->parent=t;
            }
            root = z->left;
            root->parent = nullptr;
        }
        delete(z);
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
        if(temp != nullptr) {
            e.name = temp->val.name;
            e.type = temp->val.type;
            e.scope = temp ->val.scope;
        }
        if(e.name != "null") break;
    }
    if(temp == nullptr) {
        delete temp;
        throw Undeclared(ins);
    }
    splay(temp);
    cout<<temp->val.scope<<endl;
}
void SymbolTable::insertNode(Symbol e, int &count) {
    Node *t = new Node(e);
    if (root == nullptr)
    {   cout<<count<<" "<<(count+1)/2<<endl;
        root = t;
        return;
    }
    Node *z = root;
    while (true)
    {
        if (z->val > e)
        {   count++;
            if (z->left == nullptr)
            {
                z->left = t;
                t->parent = z;
                break;
            }
            else
                z = z->left;
        }
        else if (z->val < e)
        {   count++;
            if (z->right == nullptr)
            {
                z->right = t;
                t->parent = z;
                break;
            }
            else
                z = z->right;
        }
        else
        {
            // val is already present in the three
            break;
        }
    }
    splay(t);
    cout<<count<<" "<<(count+1)/2<<endl;
    /*   int num_splay = 0;
       Node *t = new Node(e);
       if (root == nullptr)
       {   cout<<count<<" "<<num_splay<<endl;
           root = t;
           return;
       }
       Node *z = root;
       while (true)
       {
           if (z->val > e)
           {   count++;
               if (z->left == nullptr)
               {
                   z->left = t;
                   t->parent = z;
                   break;
               }
               else
                   z = z->left;
           }
           else if (z->val < e)
           {   count++;
               if (z->right == nullptr)
               {
                   z->right = t;
                   t->parent = z;
                   break;
               }
               else
                   z = z->right;
           }
           else
           {
               // val is already present in the three
               break;
           }
       }
       splay_insert(t,num_splay);
       cout<<count<<" "<<num_splay<<endl; */

}
void SymbolTable::splay_insert(Node *&z, int &nump_splay) {
    if (z == nullptr)
        return;
    while (true)
    {
        Node *par = z->parent;
        if (par == nullptr)
        {
            // z is the root
            break;
        }
        Node *gPar = par->parent;
        if (gPar == nullptr && par->left == z)
        {
            // zig
            nump_splay++;
            rightRotate(par);
        }
        else if (gPar == nullptr && par->right == z)
        {
            // zag
            nump_splay++;
            leftRotate(par);
        }
        else if (gPar->left == par && par->left == z)
        {
            // zig-zig
            nump_splay++;
            rightRotate(gPar);
            rightRotate(par);
        }
        else if (gPar->right == par && par->right == z)
        {
            // zag-zag
            nump_splay++;
            leftRotate(gPar);
            leftRotate(par);
        }
        else if (gPar->left == par && par->right == z)
        {
            // zig-zag
            nump_splay++;
            leftRotate(par);
            rightRotate(gPar);
        }
        else if(gPar->right == par && par -> left ==z)
        {
            // zag-zig
            nump_splay++;
            rightRotate(par);
            leftRotate(gPar);
        }
    }
    root = z;
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
