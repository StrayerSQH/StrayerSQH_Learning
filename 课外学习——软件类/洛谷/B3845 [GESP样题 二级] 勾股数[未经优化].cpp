//可以进行正确的运算，但是时间复杂度过高
#include <iostream>
#include <cmath>

using namespace std;

class Pythagorean_Triple
{
	private:
		int a, b, c;
		int n;
		int all = 0;
		
	public:
		void get(int x)
		{
			n = x;
		}
		
		int cal()
		{
			for (int i = 1; i <= n; i++)
			{
				for (int j = i; j <= n; j++)
				{
					for (int k = i; k <= n; k++)
					{
						if (pow(i, 2) + pow(j, 2) == pow(k, 2))
						{
							all++;
						}
					}
				}
			}
		}
		
		void print()
		{
			cout << all;
		}
};

int main()
{
	Pythagorean_Triple a;
	
	int n;
	cin >> n;
	
	a.get(n);
	a.cal();
	a.print();
	
	return 0;
}
