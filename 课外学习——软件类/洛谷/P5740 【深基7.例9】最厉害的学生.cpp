#include <iostream>
#include <cstdio>
#include<bits/stdc++.h>

using namespace std;

struct information
{
	string name;
	int yuwen, shuxue, yingyu;
};

information arr[10000];

int main()
{
	int n, max = -1, imax;
	cin >> n;
	
	for (int i = 0; i < n; i++)
	{
		cin >> arr[i].name >> arr[i].yuwen >> arr[i].shuxue >> arr[i].yingyu;
	}
	
	for (int i = 0; i < n; i++)
	{
		if (arr[i].yuwen + arr[i].shuxue + arr[i].yingyu > max)
		{
			max = arr[i].yuwen + arr[i].shuxue + arr[i].yingyu;
			imax = i;
		}
	} 
	
	cout << arr[imax].name << " " << arr[imax].yuwen << " " << arr[imax].shuxue << " " << arr[imax].yingyu;
	
	return 0;
} 
