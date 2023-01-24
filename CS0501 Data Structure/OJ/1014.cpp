#include<bits/stdc++.h>
using namespace std;
int n, _r=0, _l;
long long tot=0;
int *p, *p_2;

int main(){
    int tmp;
    cin>>n;
    p=new int[n+1]{0};
    p_2=new int[n+1]{0};
    for (int i=1;i<=n;i++)
        cin>>p[i];
    for (int i=1;i<=n;i++){
        cin>>tmp;
        if (p[i]>=tmp)
            p[i]-=tmp;
        else
            {
                p_2[i]=tmp-p[i];
                p[i]=0;
            }
    }
    
    for(int i=1;i<=n;i++){
        if (p[i]>p[i-1])
            tot+=(p[i]-p[i-1]);
        else if (p_2[i]>p_2[i-1])
            tot+=(p_2[i]-p_2[i-1]);
    }
    cout<<tot;
    delete []p;
    delete []p_2;
}