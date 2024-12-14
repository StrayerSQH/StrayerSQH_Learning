#include <iostream>
#include <string>

using namespace std;

void SR(int arr[100][100], int row1, int row2, int n)
{
	for (int i = 0; i < n; i++)
	{
		int t = arr[row1 - 1][i];
		arr[row1 - 1][i] = arr[row2 - 1][i];
		arr[row2 - 1][i] = t;
	}
}

void SC(int arr[100][100], int col1, int col2, int m)
{
	for (int i = 0; i < m; i++)
	{
		int t = arr[i][col1 - 1];
		arr[i][col1 - 1] = arr[i][col2 - 1];
		arr[i][col2 - 1] = t;
	}
}

void DR(int arr[100][100], int dr, int m, int n)
{
	for (int i = dr - 1; i < m - 1; i++)
	{
		for (int j = 0; j < n; j++)
		{
			arr[i][j] = arr[i + 1][j];
		}
	}
}

void DC(int arr[100][100], int dc, int m, int n)
{
	for (int i = dc - 1; i < n - 1; i++)
	{
		for (int j = 0; j < m; j++)
		{
			arr[j][i] = arr[j][i + 1];
		}
	}
}

void IR(int arr[100][100], int ir, int m, int n)
{
	for (int i = m - 1; i >= ir - 1; i--)
	{
		for (int j = 0; j < n; j++)
		{
			arr[i + 1][j] = arr[i][j];
		}
	}
	
	for (int i = 0; i < n; i++)
	{
		arr[ir - 1][i] = 0;
	}
}

void IC(int arr[100][100], int ic, int m, int n)
{
	for (int i = n - 1; i >= ic - 1; i--)
	{
		for (int j = 0; j < m; j++)
		{
			arr[j][i + 1] = arr[j][i];
		}
	}
	
	for (int i = 0; i < m; i++)
	{
		arr[i][ic - 1] = 00;
	}
}

class operation
{
	private:
		int m, n, t;
		int arr[100][100];
	
	public:
		void input()
		{
			cin >> m >>n;
			
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					cin >> arr[i][j];
				}
			}
		}
		
		void process()
		{
			cin >> t;
			char tmp = cin.get();
			
			for (int i = 0; i < t; i++)
			{
				char s[6];
				int len = 0;
				
        		for (int j = 0; j < 500; j++)
        		{
        			cin.get(s[j]);
        	
        			if (s[j] == '\n')
        			{
        			break;
					}
			
        			len++;
				}
				
				if (s[0] == 'S' && s[1] == 'R')
				{
					int row1 = s[3] - '0';
					int row2 = s[5] - '0';
					SR(arr, row1, row2, n); 
				}
				else if (s[0] == 'S' && s[1] == 'C')
				{
					int col1 = s[3] - '0';
					int col2 = s[5] - '0';
					SC(arr, col1, col2, m);
				}
				else if (s[0] == 'D' && s[1] == 'R')
				{
					int dr = s[3] - '0';
					DR(arr, dr, m, n);
					m--;
				}
				else if (s[0] == 'D' && s[1] == 'C')
				{
					int dc = s[3] - '0';
					DC(arr, dc, m, n);
					n--;
				} 
				else if (s[0] == 'I' && s[1] == 'R')
				{
					int ir = s[3] - '0';
					IR(arr, ir, m, n);
					m++;
				}
				else if (s[0] == 'I' && s[1] == 'C')
				{
					int ic = s[3] - '0';
					IC(arr, ic, m, n);
					n++;
				}
			}
		}
		
		
		void output()
		{
			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < n; j++)
				{
					cout << arr[i][j] << " ";
				}
				cout << endl;
			}
		}
};

int main()
{
	operation op;
	op.input();
	op.process();
	op.output();
	
	return 0;
}
