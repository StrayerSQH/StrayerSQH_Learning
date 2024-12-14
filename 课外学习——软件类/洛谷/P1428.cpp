#include <iostream>

using namespace std;
 
int main()
{
	int num;
	int arr1[100];
	int arr2[100];
	
	cin >> num;
	for (int i = 0; i < num; i++)
	{
		cin >> arr1[i];
		arr2[i] = 0;
	}
	
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if (arr1[j] < arr1[i])
			{
				arr2[i]++;
			}
		}
	}
	
	for (int i = 0; i < num; i++)
	{
		cout << arr2[i] << " ";
	}
	
	return 0;
}
