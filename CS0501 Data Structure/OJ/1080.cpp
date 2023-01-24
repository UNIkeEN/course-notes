//1080
#include<bits/stdc++.h>
using namespace std;
const int N=10001;

int main(){
    char inp[N];
    int stack[N];
    int top=-1, num=0;
    scanf("%s", inp);
    for(int i=0;i<=N;i++){
        if (inp[i]=='@') break;
        else if (inp[i]>='0' & inp[i]<='9'){
            num = num*10 +(inp[i]-'0');
        }
        else if (inp[i]=='.'){
            stack[++top]=num;
            num=0;
        }
        else{
            // cout<<stack[top-1]<<' '<<stack[top]<<endl;
            switch (inp[i]){
            case '+': stack[top-1]+=stack[top]; break;
            case '-': stack[top-1]-=stack[top]; break;
            case '*': stack[top-1]*=stack[top]; break;
            case '/': stack[top-1]/=stack[top];
            }
            stack[top--]=0;
        }
    }
    printf("%d\n",stack[0]);

}