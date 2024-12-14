#include <iostream>
#include <cstdio>
#include <cmath>
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
		for (int j = i + 1; j < n; j++)
		{
			if (abs(arr[i].yuwen - arr[j].yuwen) < 6 &&
				abs(arr[i].shuxue - arr[j].shuxue) < 6 && 
				abs(arr[i].yingyu - arr[j].yingyu) < 6 && 
				abs((arr[i].yuwen + arr[i].shuxue + arr[i].yingyu) - (arr[j].yuwen + arr[j].shuxue + arr[j].yingyu)) < 11)
			{
				if(arr[i].name > arr[j].name)
				{
					cout << arr[j].name << " " << arr[i].name << "\n";
				}
				else 
				{
					cout << arr[i].name << " " << arr[j].name << "\n";
				}
			}
		}
	} 
	
	
	return 0;
} 
