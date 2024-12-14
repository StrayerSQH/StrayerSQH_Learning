#include <iostream>

using namespace std;

int main()
{
	int n;
	cin >> n;
	int arr[n][n];
	int k = n * n;
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			arr[i][j] = 0;
		}
	}
	arr[0][n / 2] = 1;
	int a = 0; 
	int b = n / 2;
	arr[a][b];
	
	for (int i = 2; i <= k; i++)
	{
		if (a == 0 && b != n - 1)
		{
			arr[n - 1][b + 1] = i;
			a = n - 1;
			b = b + 1;
		}
		else if (a != 0 && b == n - 1)
		{
			arr[a - 1][0] = i;
			a = a - 1;
			b = 0;
		}
		else if (a == 0 && b == n - 1)
		{
			arr[1][b] = i;
			a = 1;
			b = b;
		}
		else if ((a != 0 && b != n - 1) && arr[a - 1][b + 1] == 0)
		{
			arr[a - 1][b + 1] = i;
			a = a - 1;
			b = b + 1;
		}
		else if ((a != 0 && b != n - 1) && arr[a - 1][b + 1] != 0)
		{
			arr[a + 1][b] = i;
			a = a + 1;
			b = b;
		}
	}
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << arr[i][j] <<" ";
		}
		cout << endl;
	}
	
	return 0;
} 
