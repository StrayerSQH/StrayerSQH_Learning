#include <iostream>

using namespace std;

int bingbaocaixiang (int x)
{
	if (x % 2 == 1)
	{
		x = x * 3 + 1;
	}
	else
	{
		x /= 2;
	}
	
	return x;
}

int main()
{
	int n;
	int arr[100000];
	int a = 0;
	cin >> n;
	int t = n;
	
	while (arr[a - 1] - 1)
	{
		arr[a] = bingbaocaixiang (n);
		n = arr[a];
		a++;
	}
	
	for (int i = a - 1; i >= 0; i--)
	{
		cout << arr[i] << " ";
	}
	cout << t;
	
	return 0;
}
