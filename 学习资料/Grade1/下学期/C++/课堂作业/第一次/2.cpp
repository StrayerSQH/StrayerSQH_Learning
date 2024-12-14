#include <iostream>

using namespace std;

int main()
{
	int num;
	cin >> num;
	
	int t = num;
	int digits = 0;
	int arr[1000];
	
	while (t)
	{
		arr[digits] = t % 2;
		t /= 2;
		
		digits++;
	}
	
	for (int i = digits - 1; i >= 0; i--)
	{
		if (i == digits - 1)
		{
			cout << "2^" << i;
		}
		else if (i > 0 && i != digits && arr[i] != 0)
		{
			cout << "+2^" << i;
		}
		else if (i == 0 && arr[i] != 0)
		{
			cout << "+2^0";
		}
	}
	
	return 0;
}
