#include<iostream>
using namespace std;
int a[51],b[51],c[51];//题目中有三个变量就设三个变量；
int f[501][501];
int main()
{
    int i,j,l,m,n,k;
    cin>>m>>n>>k;//输入
    for(i=1;i<=k;i++)
      cin>>a[i]>>b[i]>>c[i];//表示每个食品的体积质量和卡路里；
    for(i=1;i<=k;i++)
      for(j=m;j>=a[i];j--)
        for(l=n;l>=b[i];l--)//记住j和l不能同时写在一起，刚开始我就写在一起，调了1分钟才发现
          f[j][l]=max(f[j][l],f[j-a[i]][l-b[i]]+c[i]);//公式；直接套用就是的
      cout<<f[m][n];//输出最优解
      return 0;
}
