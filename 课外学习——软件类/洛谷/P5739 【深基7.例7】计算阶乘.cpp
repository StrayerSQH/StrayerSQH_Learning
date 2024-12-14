#include <iostream>
#include <cstdio>

using namespace std;

long long jie (int x)
{
	if (x == 1)
	{
		return 1;
	}
	
	return x * jie(x - 1);
}

int main()
{
	int n;
	
	cin >> n;
	
	long long m = jie(n);
	
	cout << m << endl;
	
	return 0;
}
