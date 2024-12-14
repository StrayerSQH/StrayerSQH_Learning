#include <iostream>
#include <cstdio>

using namespace std;

int runnian(int i)
{
	if((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
	{
		return 1;
	}
	
	return 0;
}

int main()
{
	int x, y, j = 0;
	int year[100000];
	cin >> x >> y; 
	
	for (int i = x; i <= y; i++)
	{
		if (runnian(i))
		{
			year[j] = i;
			j++;
		}
	}
	
	cout << j << "\n";
	for (int i = 0; i < j; i++)
	{
		cout << year[i] << " ";
	}
}
