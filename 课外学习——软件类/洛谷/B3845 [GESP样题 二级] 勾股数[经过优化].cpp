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
			for (int a = 1; a <= n; a++)
			{
				for (int b = a; b <= n; b++)
				{
					int c_square = a * a + b * b;
					int c = sqrt(c_square);//不为整数的话向下取整 
					if (c * c == c_square && c <= n)
					{
						all++;
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
