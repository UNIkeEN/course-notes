#include<bits/stdc++.h>
using namespace std;

char pre[30], mid[30];
char t[1010];

void buildTree(char *pre, char *mid, int len, int idx){
    if (len<=0) return;
    t[idx] = pre[0];
    int i=0;
    while(mid[i]!=pre[0]){i++;}
    buildTree(pre+1, mid, i, 2*idx);
    buildTree(pre+i+1, mid+i+1, len-i-1, 2*idx+1);
}

int main(){
    cin>>pre>>mid;
    for(int i=1;i<=1009;i++)
        t[i]='@';
    buildTree(pre, mid, strlen(pre), 1);
    int n=1000;
    while (t[n]=='@')
        n--;
    for(int i=1;i<=n;i++){
        if (t[i] == '@')
            cout<<"NULL ";
        else
            cout<<t[i]<<' ';
    }
    return 0;
}