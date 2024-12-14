#include <iostream>

using namespace std;

int main()
{
	int a, b, c;
	int arr[80];
	int all;
	 
	cin >> a >> b >> c;
	
	for (int i = 0; i < a; i++)
	{
		arr[i] = i + 1;
	}
	for (int i = a; i < a + b; i++)
	{
		arr[i] = i - a + 1;
	}
	for (int i = a + b; i < a + b + c; i++)
	{
		arr[i] = i - a - b + 1;
	}
}
