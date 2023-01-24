#include<iostream>
#include<algorithm>

using namespace std;
int n;
int **p;  //用于存储矩阵
int *arr; //用于全排列
int main(){
    int sum=0;
    cin>>n;
    p=new int*[n];
    arr = new int[n];
    for(int i=0;i<n;i++){
        p[i]=new int[n];
        arr[i]=i;
    }
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            cin>>p[i][j];
    do{
        int t=1; // 系数
        int tot=1;
        for(int i=0;i<n-1;i++){
            for(int j=i+1;j<n;j++){
                if (arr[i]>arr[j])
                t=-t;
            }
        }
        for(int i=0;i<n;i++)
            tot*=p[i][arr[i]];
        sum+=t*tot;
        // cout<<t*tot<<' ';
    }while(next_permutation(arr, arr+n));
    cout<<sum;
}