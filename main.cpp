#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
//本程序需手动输入字符串 
using namespace std;	
struct Letters{
    char c;
    Letters* next;
    Letters(char x):c(x),next(NULL){}
};
Letters* JAVAtoC(Letters* head){
    Letters *p=head;
    Letters *q=p->next;
    while(q){
        if(q->c>='A'&&q->c<='Z'){
            q->c+=32;
            Letters* temp=new Letters('_');
            p->next=temp;
            temp->next=q;
            q=temp;
            //delete temp;            //////////////////////
        }
        else {
            p=p->next;q=q->next;
        }
    }
    return head;
}
Letters* CtoJAVA(Letters* head){
    Letters *p=head;
    Letters *q=p->next;
    while(q){
        if(q->c=='_'){
            Letters *temp=q;
            q=q->next;q->c-=32;
            p->next=q; 
            delete temp;
        }
        else{
            p=p->next;
            q=q->next;
        }
    } 
    return head;
}
/////////////////////////////
void printletters(Letters *head){
    auto m = head;
    while(m){
        cout<<m->c;
        auto n = m->next; 
        delete m;
        m = n;
    }
    std::cout << std::endl;
}

void printerr(){
    std::cout << "Error!" << std::endl;
}
//////////////////////////////
void Main(Letters *head){
    //////////////////////////////////////
    //char x;
    //Letters* head=new Letters(' ');
    //Letters* q=head;
/*
    while(cin>>x){
        Letters* m=new Letters(' ');
        m->c=x;
        q->next=m;
        q=q->next;
        delete m; ///////////////// should't delete
    }
    q->next=NULL;
    */
    Letters *p=head->next;
    Letters *t=p->next;
    if (p->c == '_' || p->c >= 'A' && p->c <= 'Z'){
        printerr();
        return;
    }
    //////////////////////////////////////
    while(t){
        if(t->c=='_'){
            for(Letters* temp=t->next;temp;temp=temp->next){
                if(temp->c>='A'&&temp->c<='Z'){
                    printerr();return;
                }
            }
            //////////////////////
            auto i = head->next;
            for (; i -> next != NULL; i = i->next){
                if (i->c == '_' && i->next->c == '_'){
                    printerr();
                    return;
                }
            }
            if (i->c == '_'){
                printerr();
                return;
            }
            ///////////////////////
            Letters *newhead=CtoJAVA(head->next);
            printletters(newhead);
            return;    
        }
        else if(t->c>='A'&&t->c<='Z'){
            for(Letters* temp=t->next;temp;temp=temp->next){
                if(temp->c=='_'){
                    printerr();return;
                }
            }
            Letters *newhead=JAVAtoC(head->next);
            printletters(newhead);
            return;
        }
        else{
            p=p->next;t=t->next;
        }
    }
    printletters(head->next);
    return;
}
/////////////////////////////
int main(){
    std::string s;
    while (std::cin >> s){
        auto head = new Letters(' ');
        auto now = head;
        for (int i = 0; i < s.size(); i ++ ){
            auto tl = new Letters(s[i]);
            now->next = tl;
            now = tl;
        }
        Main(head);
    }
}
////////////////////////////
