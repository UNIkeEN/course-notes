//14151
#include<bits/stdc++.h>
using namespace std;

int main(){
    int T, n, inp, top, k;
    int *stk, *opr;
    cin>>T;
    for(int i=1;i<=T;i++){
        cin>>n;
        stk = new int[n+1];
        opr = new int[n+2];
        top=0;
        for(int j=1;j<=n;j++){
            cin>>opr[j];
        }
        k=1;
        opr[n+1]=-999;
        stk[0]=-1000;
        for(int j=1;j<=n;j++){
            stk[++top]=j;
            while(opr[k]==stk[top]) {stk[top--]=0; k++;}

        }
        printf("%s\n", (top!=0?"No":"Yes"));
        delete []stk;
        delete []opr;
    }
}