#include<bits/stdc++.h>
using namespace std;


int main(){
    bool arr[1001]={0};
    int n, tot=0, t, k=0;
    cin>>n;
    for(int i=1;i<=n;i++){
        cin>>t;
        if (!arr[t]) {arr[t]=1; tot++;}
    }
    cout<<tot<<endl;
    for(int i=1;i<=1000;i++)
        if (arr[i]) {
            if (k<tot) {cout<<i<<' '; k++;}
            else {cout<<i<<endl;}
    }
    return 0;
}