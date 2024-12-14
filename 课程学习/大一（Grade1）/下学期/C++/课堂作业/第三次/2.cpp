#include <bits/stdc++.h>

using namespace std;

int main()
{	
	string s;
	getline(cin, s);
	
	int len = s.size();
	int n = 0;
	int arr[100][100];
	int tmp[10000], t = 0, d = 0;
	
	for (int i = 0; i < len; i++)
	{
		if ('0' <= s[i] && s[i] <= '9')
		{
			n++;
			tmp[t] = s[i] - '0';
			t++;
		}
	}
	
	t = 0;
	int xia = sqrt(n);
	for (int i = 0; i < xia; i++)
	{
		for (int j = 0; j < xia; j++)
		{
			arr[i][j] = tmp[t];
			t++;
		}
	}
	
	int shang = 0, zuo = 0;
	int you = xia;
	int state = 0;
	int brr[100];
	int k = 0;
	
	cout << "[";
	for (int j = t; j > 0; j--)
	{
		if (state == 0)
		{
			for (int i = zuo; i < you; i++)
			{
				brr[k] = arr[shang][i];
				k++;
			}
			state = 1;
			shang++;
		}
		else if (state == 1)
		{
			for (int i = shang; i < xia; i++)
			{
				brr[k] = arr[i][you - 1];
				k++;
			}
			state = 2;
			you--;
		}
		else if (state == 2)
		{
			for (int i = you - 1; i >= zuo; i--)
			{
				brr[k] = arr[xia - 1][i];
				k++;
			}
			state = 3;
			xia--;
		}
		else
		{
			for (int i = xia - 1; i >= shang; i--)
			{
				brr[k] = arr[i][zuo];
				k++;
			}
			state = 0;
			zuo++;
		}
	}
	
	for (int i = 0; i < k; i++)
	{
		if (i != k - 1)
		{
			cout << brr[i] << ",";
		}
		else
		{
			cout << brr[i];
		}
	}
	cout << "]";
	
	return 0;
}
	
	
