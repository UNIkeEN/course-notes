//11617
#include<bits/stdc++.h>
using namespace std;
int ans[602];
char *m, *n;
char ansc[610];
int len1, len2;

char * add_two_string(char *m, char *n){
    int i=0, jw=0, a, b, c;
    bool mst=true, nst=true;
    do{
        if (mst and m[i]=='\0') mst = false;
        if (nst and n[i]=='\0') nst = false;
        if (!(mst or nst)) break;
        a = (mst?int(m[i])-'0':0);
        b = (nst?int(n[i])-'0':0);
        c = a+b+jw;
        jw = c/10;
        ans[i++]=(c%10);
    }while(true);
    if (jw) ans[i]=1;
    else i--;
    for(int j=0;j<=i;j++) ansc[j]=ans[j]+'0';
    ansc[i+1] = '\0';
    return ansc;
}

int main(){
    cin>>len1>>len2;
    m = new char[len1];
    n = new char[len2];
    for(int i=0;i<=len1-2;i++)
        cin>>m[i];
    m[len1-1]='\0';
    for(int i=0;i<=len2-2;i++)
        cin>>n[i];
    n[len2-1]='\0';
    char *ret = add_two_string(m,n);
    delete []m;
    delete []n;
    cout<<ret;
    // delete []ret;
    return 0;
}