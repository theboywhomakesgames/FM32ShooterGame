#include<bits/stdc++.h>
using namespace std;
int main(){
    char a[10];
    a[0]='1';
    a[1]='2';
    a[2]='3';
    int b=0;
    //sprintf(a,"%d",b);
    for(int i=0;i<strlen(a);i++){
        b=atoi(a);
    }
    cout<<b;
    return 0;
}
