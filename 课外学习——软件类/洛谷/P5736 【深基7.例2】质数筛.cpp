#include <iostream>
#include <cstdio>
#include <cstdio>
#include <cmath>

using namespace std;

int isprime(int x)
{
	if (x < 2)
	{
		return 0;
	}
	else
	{
		for (int i = 2; i <= sqrt(x); i++)
		{
			if (x % i == 0)
			{
				return 0;
			}
		}
	}
	
	return 1;
}

int main()
{
	int n;
	int arr[100];
	scanf("%d", &n);
	
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &arr[i]);
	}
	
	for (int i = 0; i < n; i++)
	{
		if (isprime(arr[i]))
		{
			printf("%d ", arr[i]);
		}
	} 
	
	return 0;
}
