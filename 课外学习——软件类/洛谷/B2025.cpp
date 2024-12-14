#include <iostream>

using namespace std;

int main()
{
	int a = 5;
	
	for (int i = 1; i <= a / 2; i++)
	{
		for (int j = a / 2 - i; j >= 0; j--)
		{
			cout<<" ";
		}
		
		for (int j = 2 * i - 1; j > 0; j--)
		{
			cout<<"*";
		}
		cout<<endl;
	}
	
	for (int i = 0; i < a; i++)
	{
		cout<<"*";
	}
	cout<<endl;
	
	for (int i = 0; i < a / 2; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			cout<<" ";
		}
		
		for (int j = a - i * 2; j > 2; j--)
		{
			cout<<"*";
		}
		cout<<endl;
	}
}
