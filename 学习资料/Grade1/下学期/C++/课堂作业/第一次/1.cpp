#include <iostream>

using namespace std;

int main()
{
	long long arr[4];
	long long sum = 0;
	
	for (int i = 0; i < 4; i++)
	{
		cin >> arr[i];
	}
	
	for (int i = 0; i < 4; i++)
	{
		sum += arr[i] * arr[i];
	}
	
	cout << sum << endl;
	
	return 0;
}
