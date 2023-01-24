#include<bits/stdc++.h>
using namespace std;

struct hfmTree{
    int weight;
    int parent;
};

int main(){
    int n;
    hfmTree *p;
    cin>>n;
    p = new hfmTree[2*n];
    for(int i=n; i<2*n; i++){
        cin>>p[i].weight;
        p[i].parent = -1;
    }
    p[0].weight = INT_MAX;
    for(int i=n-1; i>=1; i--){
        int min=0;
        int sec=0;
        for (int j=i+1; j<2*n; j++){
            if (p[j].parent==-1){
                if (p[j].weight<p[min].weight) {sec=min; min=j;}
                else if (p[j].weight<p[sec].weight) sec=j;
            }  
        }
        p[min].parent = p[sec].parent = i;
        p[i].weight = p[min].weight + p[sec].weight;
        p[i].parent = -1;
    }
    int ans = 0;
    for(int i=1; i<=n-1; i++){
        ans += p[i].weight;
    }
    cout<<ans<<endl;
    delete []p;
}