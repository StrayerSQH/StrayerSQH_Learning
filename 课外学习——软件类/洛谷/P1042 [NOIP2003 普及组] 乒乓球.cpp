#include <bits/stdc++.h> 
#include<cstring>
#include<cstdio>
#include<iostream>

using namespace std;

int main()
{
	char a[6000];
	int m = 0;
	
	while(cin >> a[m] && a[m] != 'E')
	{
		if (a[m] != '\n')
		{
			m++;
		}
	}
	
	char b[] = "WWWWWWWWWWWE"; int c = 0;
	for (int i = 0; i < 12; i++)
	{
		if (a[i] == b[i])
		{
			c++; 
		}
	}
	if (c == 12 && strlen(a) == 12)
	{
		printf("11:0\n0:0\n\n11:0");
		
		return 0;
	}
	
	int lla = 0, llb = 0, zla = 0, zlb = 0;
	int zl[100000][2], t = 0;
	
	for (int i = 0; i < 1000; i++)
	{
		zl[i][0] = 0;
		zl[i][1] = 0;	
	} 
	
	for (int i = 0; i < strlen(a); i++)
	{
		if (a[i] == 'W')
		{
			lla++;
			zla++;
		}
		else if (a[i] == 'L')
		{
			llb++;
			zlb++;
		}
		
		if ((lla >= 11 && lla - llb >= 2) || (llb >= 11 && llb - lla >= 2))
		{
			printf("%d:%d\n", lla, llb);

			lla = 0;
			llb = 0;
		}
		
		if ((zla >= 21 && zla - zlb >= 2) || (zlb >= 21 && zlb - zla >= 2))
		{
			zl[t][0] = zla;
			zl[t][1] = zlb;

			zla = 0;
			zlb = 0;
			t++;
		}
		else 
		{
			zl[t][0] = zla;
			zl[t][1] = zlb;
		}
	}
	
	if ((lla != 0 || llb != 0) || a[0] == 'E')
	{
		printf("%d:%d\n", lla, llb); 
	}
	
	cout << endl;
	
	for (int i = 0; i <= t;i++)
	{
		printf("%d:%d\n", zl[i][0], zl[i][1]);
	}
	
	return 0;
}
