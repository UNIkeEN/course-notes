#include<iostream>
using namespace std;

void qsort(int a[],int l,int r)
{
    int mid=a[(l+r)/2];
    int i=l,j=r;
    do{
        while(a[i]<mid) i++;
        while(a[j]>mid) j--;
        if(i<=j)
        {
            swap(a[i],a[j]);//交换
            i++;
            j--;
        }
    }while(i<=j);
    if(l<j) qsort(a,l,j);
    if(i<r) qsort(a,i,r);
}


int main(){
    int n, t, k, sum=0, i;
    double ans;
    int *s;
    cin>>n>>t>>k;
    s = new int[n];
    for(i=0;i<n;i++) cin>>s[i];
    qsort(s, 0, n-1);
    t=k-t;
    for(i=0;i<n;i++){
        sum+=s[i];
        if(sum>=t) break;
    }
    ans=i*(100.0/n)+(n-i)*(10.0/n);
    cout<<(int)ans;
}