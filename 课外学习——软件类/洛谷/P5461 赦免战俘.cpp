#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

int main()
{
	int n;
	cin >> n;
	int m = pow(2, n);
	int arr[m][m];
	
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			arr[i][j] = 0;
		}
	}
	
	int q[500][500];
	q[0][0] = 1;
	int t[500][500];
	for (int i = 1; i <= n; i++)
    {
    	int a = pow(2, i);
    	int zero[a / 2][a / 2];
    		
    	////�������Ͻǵ�0���� 
    	for (int j = 0; j < a / 2; j++)
    	{
    		for(int k = 0; k < a / 2; k++)
			{
				zero[j][k] = 0;
			} 
		}
    	////ƴ��Ϊһ�������ľ���
		//ƴ�����Ͻ� 
		for (int j = 0; j < a / 2; j++)
		{
			for(int k = 0; k < a / 2; k++)
			{
				t[j][k] = zero[j][k];
			}
		} 
		//ƴ�����Ͻ�
		for (int j = 0; j < a / 2; j++)
		{
			for(int k = a / 2; k < a; k++)
			{
				t[j][k] = q[j][k - a / 2];
			}
		}
		//ƴ�����½�
		for (int j = a / 2; j < a; j++) 
		{
			for(int k = 0; k < a / 2; k++)
			{
				t[j][k] = q[j - a / 2][k];
			}
		}
		//ƴ�����½�
		for (int j = a / 2; j < a; j++) 
		{
			for(int k = a / 2; k < a; k++)
			{
				t[j][k] = q[j - a / 2][k - a / 2];
			}
		}
		
		////����q
		for (int j = 0; j < a; j++)
		{
			for (int k = 0; k < a; k++)
			{
				q[j][k] = t[j][k];
			}
		 } 
	}
	
    for (int i = 0; i < pow(2, n); i++)
    {
    	for (int j = 0; j < pow(2, n); j++)
    	{
    		cout << t[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
