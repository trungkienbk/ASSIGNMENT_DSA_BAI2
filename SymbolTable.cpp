#include "SymbolTable.h"
using namespace std;
void SymbolTable::run(string filename)
{
    int cur_level = 0;
    ifstream f(filename);
    while (!f.eof()){
        string ins;
        getline(f, ins);
        if(regex_match(ins,ins_vari)) {     ///// HAM INSERT BIEN
            insert_value(ins,cur_level);
        }
        else if (regex_match(ins,ins_func)){ ///// HAM INSERT FUNCTION
            insert_func(ins,cur_level);
        }
        else if (regex_match(ins, ass_val)){ //// HAM ASSIGN VALUE
            assign_value(ins,cur_level);
        }
        else if(regex_match(ins, ass_vari)){ //// HAM ASSIGN VARIABLE
            assign_variable(ins,cur_level);
        }
        else if(regex_match(ins,ass_func)){
            assign_func(ins,cur_level);
        }
        else if(regex_match(ins,look_up)){
            string name;
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
            DList store_order;
            while(dList.head && dList.head->val.scope == cur_level){
                store_order.push(dList.head->val);
                dList.pop();
            }
            while(store_order.head && store_order.head->val.scope == cur_level){
                Node *pDel = searchLevell(store_order.head->val.name,store_order.head->val.scope);
              //  removeTree(store_order.head->val);
                remove(root,pDel);
                store_order.pop();
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
    //s+=cur->val.name + "//" + to_string(cur->val.scope)+"-->"+cur->val.decodes+ "-->" +cur->val.type+" ";
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
        return;
    } else {
        s.pop_back();
    }
    cout<<s<<endl;
}
void SymbolTable::rightRotate(Node *&curr) {
    Node *l = curr->left;
    Node *c = l->right;
    Node *p = curr->parent;
    if (p != nullptr)
    {
        if (p->left == curr)
            p->left = l;
        else
            p->right = l;
    }
    l->parent = p;
    l->right = curr;
    curr->parent = l;
    curr->left = c;
    if (c != nullptr)
        c->parent = curr;
}
void SymbolTable::leftRotate(Node *&curr) {
    Node *r = curr->right;
    Node *c = r->left;
    Node *p = curr->parent;
    if (p != nullptr)
    {
        if (p->left == curr)
            p->left = r;
        else
            p->right = r;
    }
    r->parent = p;
    r->left = curr;
    curr->parent = r;
    curr->right = c;
    if (c != nullptr)
        c->parent =curr;
}
void SymbolTable::splay(Node *&z) {
    if (z == nullptr)
        return;
    while (true)
    {
        Node *parr = z->parent;
        if (parr == nullptr)
        {
            // z is the root
            break;
        }
        Node *gPar = parr->parent;
        if (gPar == nullptr && parr->left == z)
        {
            // zig
            rightRotate(parr);
        }
        else if (gPar == nullptr && parr->right == z)
        {
            // zag
            leftRotate(parr);
        }
        else if (gPar->left == parr && parr->left == z)
        {
            // zig-zig
            rightRotate(gPar);
            rightRotate(parr);
        }
        else if (gPar->right == parr && parr->right == z)
        {
            // zag-zag
            leftRotate(gPar);
            leftRotate(parr);
        }
        else if (gPar->left == parr && parr->right == z)
        {
            // zig-zag
            leftRotate(parr);
            rightRotate(gPar);
        }
        else if(gPar->right == parr && parr-> left ==z)
        {
            // zag-zig
            rightRotate(parr);
            leftRotate(gPar);
        }
    }
    root = z;
}
Node* SymbolTable::searchLevell(string name, int level) {
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
void SymbolTable::lookup(string name, int level, string ins) {
    if(isValidId(name) == false) throw InvalidInstruction(ins); // Name la key word
    if(root == nullptr) throw Undeclared(ins);
    Symbol e("null","null",-1);
    Node *temp = nullptr;
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
    {   cout<<count<<" "<<"0"<<endl;
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
    cout<<count<<" "<<"1"<<endl;
}
Node* SymbolTable::isContains(string name, int level,int &num_comp,int &num_splay) {
    int temp_comp= num_comp;
    int temp_splay = num_splay;
    Node *temp = nullptr;
    if(root == nullptr) return nullptr;
    for(int i = level; i >= 0 ;--i){
        num_comp = temp_comp;
        num_splay = temp_splay;
        temp = searchLevell_assign(name,i,num_comp,num_splay);
        if(temp!= nullptr) break;
    }
    return temp;
}
Node* SymbolTable::searchLevell_assign(string name, int level, int &num_comp,int &num_splay) {
    Symbol *e= new Symbol(name,"null",level);
    if (root == nullptr){
        delete e;
        return nullptr;
    }
    Node *cur=root;
    while (true)
    {
        if (cur->val == *e) {
            num_comp++;
            break;
        }
        else if (cur->val > *e){
            num_comp++;
            if (cur->left == nullptr)
                break;
            else {
                cur = cur->left;
            }
        }
        else{
            num_comp++;
            if (cur->right == nullptr)
                break;
            else {
                cur = cur->right;
            }
        }
    }
    if (cur->val == *e){
        if(cur!=root) num_splay++;
        splay(cur);
        delete e;
        return cur;
    }
    else{
        delete e;
        return nullptr;
    }
}
void SymbolTable::assign_func(string ins,int cur_level) {
    if(isKeyword(ins)) throw InvalidInstruction(ins);
    string id,value_func,func_name,argu;  // value_func = func_name + argu
    int num_comp = 0;
    int num_splay = 0;
    int count_point = 0;
    int index[2]={0,0};
    int index_par[40];
    int j = 0;
    for(int i=0;i<(int)ins.size();++i){
        if(j==2) break;
        if(ins[i]==' '){
            index[j]=i;
            j++;
        }
    }
    id = ins.substr(index[0]+1,index[1]-index[0]-1);
    value_func  = ins.substr(index[1]+1);
    int find_parless = (int)value_func.find('(');
    func_name = value_func.substr(0,find_parless); //// Name of function
    argu = value_func.substr(find_parless);   //// List variable (...)
    Node *temp_func = isContains(func_name,cur_level,num_comp,num_splay);
    if(temp_func == nullptr){
        throw Undeclared(ins);
    }
    if(temp_func->val.decodes == "") throw TypeMismatch(ins); //// is variabe
    if(temp_func->val.decodes.length() == 1){ //// No argument
        //
    }
    else {
        for(char i : argu){
            if(i == ','){
                count_point++;
            }
        }
        if(count_point + 2 != (int) temp_func->val.decodes.length()) throw TypeMismatch(ins);
        count_point = 0;
        for(int i = 0 ;i<(int) argu.length();++i){
            if(argu[i] == '(' ||argu[i] == ')' ||argu[i] == ','){
                index_par[count_point] = i;
                count_point++;
            }
        }
        int k = 1;
        for(int i = 0 ; i< count_point-1;++i){
            string temp = argu.substr(index_par[i]+1,index_par[i+1]-index_par[i]-1);
            if(temp[0]>='0'  &&temp[0]<='9'){
                if(temp_func->val.decodes[k] != '0') throw TypeMismatch(ins);
            }
            else if(temp[0]=='\''){
                if(temp_func->val.decodes[k] != '1') throw TypeMismatch(ins);
            }
            else {
                Node *temp_argu = isContains(temp,cur_level,num_comp,num_splay);
                if(temp_argu == nullptr) throw Undeclared(ins);
                if(temp_argu->val.decodes != "") throw TypeMismatch(ins);
                if(temp_argu->val.type =="number" && temp_func->val.decodes[k]=='1') throw TypeMismatch(ins);
                if(temp_argu->val.type =="string" && temp_func->val.decodes[k]=='0') throw TypeMismatch(ins);
            }
            ++k;
        }
    }
    Node *temp_id = isContains(id,cur_level,num_comp,num_splay);
    if(temp_id == nullptr) throw Undeclared(ins);
    if(temp_id->val.decodes != "") throw TypeMismatch(ins);
    if(temp_id->val.type != temp_func->val.type) throw TypeMismatch(ins);
    cout<<num_comp<<" "<<num_splay<<endl;
}
void SymbolTable::assign_value(string ins, int cur_level) {
    string id,valu;
    int num_comp = 0;
    int num_splay = 0;
    int index[2]={0,0};
    int j = 0;
    for(int i=0;i<(int)ins.size();++i){
        if(j==2) break;
        if(ins[i]==' '){
            index[j]=i;
            j++;
        }
    }
    id = ins.substr(index[0]+1,index[1]-index[0]-1);
    valu = ins.substr(index[1]+1);
    string valu_type;
    if(isValidId(id) == false) throw InvalidInstruction(ins); // Name la key word
    if(valu[0]=='\'') valu_type = "string";
    else valu_type = "number";
    Node *temp = isContains(id,cur_level,num_comp,num_splay);
    if(temp == nullptr) throw Undeclared(ins);
    if(temp->val.decodes[0] == '(') throw TypeMismatch(ins);
    if(temp->val.type != valu_type){
        throw TypeMismatch(ins);
    }
    cout<<num_comp<<" "<<num_splay<<endl;
}
void SymbolTable::assign_variable(string ins, int cur_level) {
    string id,valu;
    int num_comp = 0;
    int num_splay = 0;
    int index[2]={0,0};
    int j = 0;
    for(int i=0;i<(int)ins.size();++i){
        if(j==2) break;
        if(ins[i]==' '){
            index[j]=i;
            j++;
        }
    }
    id = ins.substr(index[0]+1,index[1]-index[0]-1);
    valu = ins.substr(index[1]+1);
    if(isValidId(id) == false) throw InvalidInstruction(ins); // Name la key word
    if(isValidId(valu) == false) throw InvalidInstruction(ins); // Name la key word
    Node *temp_valu = isContains(valu,cur_level,num_comp,num_splay);
    if(temp_valu == nullptr) throw Undeclared(ins);
    if(temp_valu->val.decodes !="") throw TypeMismatch(ins);
    Node *temp_id = isContains(id,cur_level,num_comp,num_splay);
    if(temp_id == nullptr) {
        throw Undeclared(ins);
    }
    if(temp_id->val.decodes !="") throw TypeMismatch(ins);
    if(temp_id->val.type != temp_valu->val.type) throw TypeMismatch(ins);
    cout<<num_comp<<" "<<num_splay<<endl;
}
void SymbolTable::insert_value(string ins, int cur_level) {
    string name,var,isStatic,decode;
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
    if(isValidId(name) == false) throw InvalidInstruction(ins); // Name la key word
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
void SymbolTable::insert_func(string ins,int cur_level) {
    string name,var,isStatic,decode;
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
    string decode_tmp = "(";
    if(isValidId(name) == false) throw InvalidInstruction(ins); // Name la key word
    for(int i = 0 ; i < (int) decode.length() ;++i){ // number -> 0 , string -> 1
        if(decode[i] == 'm') decode_tmp+="0";
        if(decode[i] == 's') decode_tmp+="1";
    }
    if(decode_tmp[decode_tmp.length()-1] == '0'){
        var = "number";
    }
    else var = "string";
    decode_tmp.pop_back();
    Symbol e(name,var,cur_level);
    if(isStatic == "true"){
        e.scope = 0;
    } else {
        if(cur_level > 0){
            throw InvalidDeclaration(ins);
        }
    }
    Node *temp = searchLevell(e.name,e.scope);
    if(temp== nullptr){
        e.decodes = decode_tmp;
        insertNode(e,count);
        dList.push(e);
        if(dList.head->val.scope == 0){
            dList.pop();
        }
    }
    else {
        throw Redeclared(ins);
    }
}
void SymbolTable::DestroyTree(Node *node) {
    if (node)
    {
        DestroyTree(node->left);
        DestroyTree(node->right);
        delete node;
    }
    root= nullptr;
}
bool SymbolTable::isValidId(string id) {
    if(id =="true" ||  id == "false"|| id =="string" || id =="number") return false;
    return true;
}
bool SymbolTable::isKeyword(const string &ins) {
    string id, valu;
    stringstream ss(ins);
    ss >> id >> id;
    getline(ss,valu);
    valu = valu.substr(1);
    if(!isValidId(id)) return true;
    if(valu[valu.size()-1]==')') {
        size_t posi_S = 0;
        size_t posi_E = valu.find('(');

        string funcIden = valu.substr(posi_S, posi_E);
        if(!isValidId(funcIden)) return true;

        if(valu[valu.size()-2] != '(') {
            posi_S = posi_E+1;
            posi_E = valu.find(',', posi_S);

            while(posi_E != string::npos) {
                string paraName = valu.substr(posi_S, posi_E-posi_S);
                if(!isValidId(paraName)) return true;
                posi_S = posi_E+1;
                posi_E = valu.find(',', posi_S);
            }
            string argu_name = valu.substr(posi_S, valu.size() - posi_S - 1);
            if(!isValidId(argu_name)) return true;
        }

    } else {
        if(!isValidId(valu)) return true;
    }
    return false;
}
