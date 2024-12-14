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
	
	return x;
}

int main()
{
	int n, a = 0;
	cin >> n;
	int arr[100000];
	
	for (int i = 2; i < n; i++)
	{
		arr[a] = prime(i);
		a++;
	}
	
	for (int i = 4; i <= n; i += 2)
	{
		for (int j = 0; j < n; j++)
		{
			if (prime(i - arr[j]))
			{
				cout << i << "=" << arr[j] << "+" << i - arr[j] << endl;
				break;
			}
		}
	}
	return 0;
}
