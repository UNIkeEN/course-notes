// #include<bits/stdc++.h>
// using namespace std;

// int n,m;
// struct sct{
//     int value;
//     int pos;
// };
// sct *data, *ans;
// int main(){
//     int t,len=0, tot=0;
//     bool flag;
//     cin>>n;
//     data = new sct[n+1];
//     for(int i=0;i<n;i++)
//         data[i].value=INT_MIN;
//     for(int i=1;i<=n;i++){
//         cin>>t;
//         flag=true;
//         for(int j=1;j<=len;j++)
//             if (data[j].value==t){
//                 flag=false;
//                 break;
//             }
//         if (flag){
//             len++;
//             data[len].value=t;
//             data[len].pos=i;
//         }
//     }
//     cin>>m;
//     ans = new sct[m];
//     for(int i=0;i<m;i++){
//         cin>>ans[i].value;
//         ans[i].pos=0;
//     }
//     for(int i=0;i<m;i++){
//         if (ans[i].pos==0){
//             flag=false;
//             for(int j=1;j<=len;j++)
//                 if (data[j].value==ans[i].value){
//                     ans[i].pos=j; flag=true; break;
//                 }
//             if (flag){
//                 t=0;
//                 for(int k=i+1;k<m;k++){
//                     t+=1;
//                     if (ans[k].value==ans[i].value){
//                         ans[k].pos=t; t=0;
//                     }
//                 }
//             }
//             else{
//                 ans[i].pos=len;
//                 for(int k=i+1;k<m;k++)
//                     if (ans[k].value==ans[i].value)
//                         ans[k].pos=len;
//             }
//         }
//     }
//     for(int i=0;i<m;i++)
//         tot+=ans[i].pos;
//     cout<<tot;
//     return 0;
// }

#include<bits/stdc++.h>
using namespace std;

struct node{
    int value;
    node* next;
};

int n, t, m;
node *head, *p, *q;

int search(int x){
    p=head;
    int i;
    node *tmp;
    bool fl=false;
    for(i=1;i<=n;i++){
        q=p->next;
        if (q->value==x){
            p->next=q->next;
            delete q;
            q=new node;
            q->value=x;
            q->next=head->next;
            head->next=q;
            fl=true;
            break;
        }
        p=p->next;
    }
    if (fl) return i;
    return i-1;
}

int main(){
    int tot=0;
    head=new node;
    p=head;
    cin>>n;
    for(int i=1; i<=n; i++){
        q=new node;
        p->next=q;
        p=q;
        cin>>t;
        p->value=t;
    }
    cin>>m;
    for(int i=1;i<=m;i++){
        cin>>t;
        tot+=search(t);
    }
    cout<<tot;
}