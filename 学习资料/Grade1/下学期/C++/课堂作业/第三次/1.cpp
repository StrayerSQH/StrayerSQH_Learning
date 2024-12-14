#include <bits/stdc++.h>

using namespace std;

int main()
{	
	string s;
	getline(cin, s);
	
	int len = s.size();
	int n = 0;
	int arr[100][100];
	int brr[100][100];
	int tmp[10000], t = 0;
	
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
	for (int i = 0; i < sqrt(n); i++)
	{
		for (int j = 0; j < sqrt(n); j++)
		{
			arr[i][j] = tmp[t];
			t++;
		}
	}
	
	for (int i = 0; i < sqrt(n); i++) 
	{  
        for (int j = 0; j < sqrt(n); j++) 
		{  
            brr[j][i] = arr[i][j];  
        }  
    } 
    
    for (int i = 0; i < sqrt(n) / 2; i++)
    {
    	for (int j = 0; j < sqrt(n); j++)
    	{
    		int tmp;
    		int a = sqrt(n) - i - 1;
    		tmp = brr[j][a];
    		brr[j][a] = brr[j][i];
    		brr[j][i] = tmp;
		}
	}
	
	for (int i = 0; i < sqrt(n); i++)
	{
		for (int j = 0; j < sqrt(n); j++)
		{
			cout << brr[i][j];
		}
		cout << endl;
	}
	 
	return 0;
} 
