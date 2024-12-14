#include <iostream>

using namespace std;

class sol
{
	private:
		int n;
	
	public:
		void pro()
		{
			cin >> n;
			
			for (int i = 0; i < n; i++)
			{
				int num;
				cin >> num;
				int b = 0;
				int c = num;
				
				while (c > 0)
				{
					b = b * 10 + c % 10;
					c /= 10;
				}
				
				if (b == num)
				{
					cout << "Yes" << endl;
				}
				else
				{
					cout << "No" << endl;
				}
			}
		}
};

int main()
{
	sol a;
	a.pro();
}
