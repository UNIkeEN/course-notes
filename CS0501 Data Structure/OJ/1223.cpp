#include<bits/stdc++.h>
using namespace std;

long n,k;
bool flag=true;

void _search(long x1, long y1, long x2, long y2, long &k){
    if (!flag) return;
    if ((y1+y2)<=n){
        _search(x1+x2,y1+y2,x2,y2,k);
        if (k==1 && flag){
            cout<<(x1+x2)<<'/'<<(y1+y2);
            flag=false;
            return;
        }
        else k--;
        _search(x1,y1,x1+x2,y1+y2,k);
    }
}

int main(){
    cin>>n>>k;
    _search(0,1,1,1,k);
    return 0;
}