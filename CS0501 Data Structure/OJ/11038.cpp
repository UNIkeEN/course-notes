// 11038
#include<bits/stdc++.h>
using namespace std;

int main(){
    struct linknode{
        int data;
        linknode *next;
    };
    int n;
    int *k;
    linknode *head, *p, *tmp;
    cin>>n;
    k = new int[n];
    for(int i=1;i<n;i++)
        cin>>k[i];
    head = new linknode;
    p = head;
    p->data = 1;
    for(int i=1;i<n;i++){
        tmp = new linknode;
        tmp->data = i+1;
        p->next = tmp;
        p = tmp;
    }
    p->next = head;
    for(int i=1;i<n;i++){
        k[i]=(k[i]-1)%(n-i+1);
        for(int j=1;j<=k[i];j++)
            p = p->next;
        tmp = p->next;
        p->next = tmp->next;
        delete tmp;
    }
    cout<<p->data;
    
    delete []k;
}