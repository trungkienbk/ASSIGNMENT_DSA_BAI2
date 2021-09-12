#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
using namespace std;
class Symbol{
public:
    string name;
    string type;
    int scope;
public:
    Symbol(){};

    Symbol(const string &name, const string &type, int scope) : name(name), type(type), scope(scope) {}

    ~Symbol() {
    }
};
class SymbolTable
{
private:
    class Node;
private:
    Node *head;
    Node *tail;
    int count;
public:
    SymbolTable() : head(nullptr), tail(nullptr), count(0) {}
    ~SymbolTable()
    {
        this->clear();
    }
    void run(string filename);

    int size()
    {
        return this->count;
    }
    bool empty()
    {
        if(this->count==0) return true;
        return false;
    }

    void clear()
    {
        while(head){
            Node *cur = head;
            head=head->next;
            delete cur;
            this->count--;
        }
        tail= nullptr;
    }
    void push(Symbol symbol){
        Node* pNew = new Node(symbol);
        if (!head)
        {
            head = pNew;
            tail = head;
            this->count++;
            return;
        }
        else {
            pNew->next = head;
            head->prev = pNew;
            head = pNew;
            count++;
            return;
        }
    };
    void pop(){
        if(this->head== nullptr) return;
        Node *temp = head;
        if(count==1){
            head= nullptr;
            tail=nullptr;
        }
        else {
            head=head->next;
            head->prev = nullptr;
        }
        delete temp;
        this->count--;
    };
    void show(){
        cout<<"LIST : ";
        Node *temp = head;
        while(temp!=nullptr){
            cout << temp->value.name <<"//"<<temp->value.scope<<" ";
            temp=temp->next;
        }
        cout<<endl;
    }
    Symbol search(string name,int level) {
        Node *temp = head;
        while (temp != nullptr) {
            if (temp->value.name == name && temp->value.scope <= level) {
                Symbol check(temp->value.name,temp->value.type,temp->value.scope); // tim thay
                return check;
            }
            temp=temp->next;
        }
        Symbol check("null","null",-1);
        return check;
    }
    void pop_scope(int level){
        for (;head;pop()){
            if(head->value.scope != level) break;
        }
    }
    bool isContains(Symbol symbol,SymbolTable &table) {
        Node *temp = table.head;
        while (temp != nullptr) {
            if (symbol.name == temp->value.name ) {
                return true;
            }
            temp=temp->next;
        }
        return false;
    }
    void print(){
        SymbolTable table;
        Node *temp = this->head;
        while (temp != nullptr){
            bool contains =table.isContains(temp->value,table);
            if(contains){
              // continue;
            }
            else {
                Symbol check(temp->value.name,temp->value.type,temp->value.scope);
               // Symbol check = this->search(temp->value.name,level);
                if(check.scope == -1){
                    //
                } else {
                    table.push(temp->value);
                }
            }
            temp=temp->next;
        }
        if(table.size() == 0) return;
        Node *tmp = table.head;
        while (tmp->next !=  nullptr){
            cout<<tmp->value.name<<"//"<<tmp->value.scope<<" ";
            tmp=tmp->next;
        }
        cout<<tmp->value.name<<"//"<<tmp->value.scope<<endl;
    }
    void rprint(){
        SymbolTable table;
        Node *temp = this->head;
        while (temp != nullptr){
            bool contains =table.isContains(temp->value,table);
            if(contains){
                // continue;
            }
            else {
                Symbol check(temp->value.name,temp->value.type,temp->value.scope);
                // Symbol check = this->search(temp->value.name,level);
                if(check.scope == -1){
                    //
                } else {
                    table.push(temp->value);
                }
            }
            temp=temp->next;
        }
        if(table.size() == 0) return;
        Node *tmp = table.tail;
        while (tmp->prev !=  nullptr){
            cout<<tmp->value.name<<"//"<<tmp->value.scope<<" ";
            tmp=tmp->prev;
        }
        cout<<tmp->value.name<<"//"<<tmp->value.scope<<endl;
    }
    void assign(string name,string var, int level,string ins);
private:
    class Node
    {
    public:
        Symbol value;
        Node *prev = nullptr;
        Node *next = nullptr;
        Node(Symbol element){
            this->value=element;
        }
    };
};
regex re_num("[0-9]*");
regex re_str("'[a-zA-Z0-9 ]*'");
regex in("(INSERT )([a-z][a-zA-Z_\\d]*)(( number)|( string))");
regex as("(ASSIGN )([a-z][a-zA-Z_\\d]*)( )((\'[a-zA-Z\\d ]+\')|(\\d+)|([a-z][a-zA-Z_\\d]*))");
regex lk("(LOOKUP )([a-z][a-zA-Z_\\d]*)");
regex ty("(number$)|(string$)");
regex na("[a-z][a-zA-Z\\d_]*");
regex num("\\d+$");
regex str("\'([a-zA-Z0-9 ]*)\'");
#endif