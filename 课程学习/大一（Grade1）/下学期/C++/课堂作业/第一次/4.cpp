#include <iostream>
#include <cstring>

using namespace std;

int main()
{
	int n, m, q;
	char arr[50][50];
	
	cin >> n >> m;
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> arr[i][j];
		}
	}
	
	cin >> q;
	
	for (int i = 0; i < q; i++)
	{
		int x, y;
		int step;
		
		cin >> x >> y >> step;
		
		x -= 1;
		y -= 1;
		for (int i = 0; i < step; i++)
		{
			if (arr[x][y] == 'U')
			{
				if (x != 0)
				{
					x -= 1;
				}
				else
				{
					x = n - 1;
				}
			}
			else if (arr[x][y] == 'D')
			{
				if (x != n - 1)
				{
					x += 1;
				}
				else
				{
					x = 0;
				}
			}
			else if (arr[x][y] == 'L')
			{
				if (y != 0)
				{
					y -= 1;
				}
				else
				{
					y = m - 1;
				}
			}
			else
			{
				if (y != m - 1)
				{
					y += 1;
				}
				else
				{
					y = 0;
				}
			}
		}
	
		x += 1;
		y += 1;
		
		cout << x << " " << y << endl;
	}
	
	return 0;
}
