#include <iostream>

using namespace std;

int main()
{
	char ch;
	
	cin >> ch;
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3 - i; j > 1; j--)
		{
			cout << " ";
		}
		
		for (int j = i * 2 + 1; j > 0; j--)
		{
			cout << ch;
		}
		
		cout << endl;
	}
	
	return 0;
}
