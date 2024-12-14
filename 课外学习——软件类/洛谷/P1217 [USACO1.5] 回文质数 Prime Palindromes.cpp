#include <bits/stdc++.h>

using namespace std;

int prime(int x)
{
	for (int i = 2; i <= sqrt(x); i++)
	{
		if (x % i == 0)
		{
			return 0;
		}
	}
	
	return 1;
}

int echo(int x)
{
	int a = 0;
	int b = x;
	while (b > 0)
	{
		a = a * 10 + b % 10;
		b /= 10;
	}
	
	if (a == x)
	{
		return 1;
	}
	
	return 0;
}

int main()
{
	int a, b, c = 0;
	
	cin >> a >> b;
	
	int arr[250000];
	for (int i = a; i <= b; i++)
	{
		if (echo(i))
		{
			arr[c] = i;
			c++;
		}
	}
	for (int i = 0; i < c; i++)
	{
		if(prime(arr[i]))
		{
			cout << arr[i] << endl;
		}
	}
}
