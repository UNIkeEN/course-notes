#include<cstdio>
#include<iostream>
using namespace std;

const int N=10010;
string s;
string val[N];
long long _hash[N];
int n,ans=0;

long long calc(){
	int len=s.size();
	long long rec=0;
	for(int i=0;i<len;i++){
		rec=rec*233317+s[i];
		rec%=212370440130137957ll;
	}
	return rec;
}

int main(){
	cin>>n;
	getchar();
	for(int i=1;i<=n;i++){
		cin>>s;
		int h=0, j=0;
		h=calc();
        //cout<<h<<endl;
		for(j=1;j<=ans;j++){
            if (h==_hash[j]) break;
        }
        if (j==ans+1){
            _hash[++ans]=h;
        }
	}
	cout<<ans;
	return 0;
}
