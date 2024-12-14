#include <iostream>

using namespace std;

int main()
{
	int arr[100];
	int num = 0;
	
	for (int i = 0; i < 100; i++)
	{
		cin >> arr[i];
		if (arr[i] != 0)
		{
			num++;
		}
		else
		{
			break;
		}
	} 
	
	for (int i = num - 1; i >= 0; i--)
	{
		cout << arr[i] << " ";
	}
}
