#include<iostream>
using namespace std;
int a[51],b[51],c[51];//��Ŀ��������������������������
int f[501][501];
int main()
{
    int i,j,l,m,n,k;
    cin>>m>>n>>k;//����
    for(i=1;i<=k;i++)
      cin>>a[i]>>b[i]>>c[i];//��ʾÿ��ʳƷ����������Ϳ�·�
    for(i=1;i<=k;i++)
      for(j=m;j>=a[i];j--)
        for(l=n;l>=b[i];l--)//��סj��l����ͬʱд��һ�𣬸տ�ʼ�Ҿ�д��һ�𣬵���1���Ӳŷ���
          f[j][l]=max(f[j][l],f[j-a[i]][l-b[i]]+c[i]);//��ʽ��ֱ�����þ��ǵ�
      cout<<f[m][n];//������Ž�
      return 0;
}
