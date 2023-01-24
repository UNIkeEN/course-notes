#include<iostream>
using namespace std;

long divid(long a[], long start, long end){
    long k=a[start];
    do{
        while (start<end && a[end]>=k) end--;
        if (start<end) {a[start]=a[end]; start++;}
        while (start<end && a[start]<=k) start++;
        if (start<end) {a[end]=a[start]; end--;}
    }while(start!=end);
    a[start]=k;
    return start;
}

void qsort(long a[], long start, long end){
    long mid;
    if (start>=end) return;
    mid = divid(a, start, end);
    qsort(a, start, mid-1);
    qsort(a, mid+1, end);
}

int main(){
    long n;
    long *a,*b;
    cin>>n;
    a = new long[n];
    b = new long[n];
    for(long i=0;i<n;i++) cin>>a[i];
    for(long i=0;i<n;i++) cin>>b[i];
    long long sum1=0, sum2=0;
    qsort(a,0,n-1);
    qsort(b,0,n-1);
    for(long i=0;i<n;i++){
        sum1+=(a[i]*b[i]);
        sum2+=(a[i]*b[n-1-i]);
    }
    cout<<sum1<<' '<<sum2<<endl;
    delete []a;
    delete []b;
    return 0;
}