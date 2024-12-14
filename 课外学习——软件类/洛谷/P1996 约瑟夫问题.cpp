#include <iostream>

using namespace std;

class Joseph_Question
{
	private:
		int n, m;
		int arr[101];
	
	public:
		void get(int x, int y, int inputArr[])
		{
			n = x;
			m = y;
			for (int i = 0; i < n; i++) {
				arr[i] = inputArr[i];
			}
		}
		
		void Joseph()
		{
			int count = 0;
			int put_num = 0;
			for(int i = 0; put_num < n; i++)
			{
				if (arr[i] != 0)
				{
					count++;
				}
				
				if (count == m)
				{
					cout << arr[i] << " ";
					put_num++;
					arr[i] = 0;
					count = 0;
				}
				
				if (i == n - 1)
				{
					i = -1;
				}
			}
		}
};

int main()
{
	Joseph_Question a;
	
	int n, m;
	int arr[101];
	cin >> n >> m;
	
	for (int i = 0; i < n; i++)
	{
		arr[i] = i + 1;
	}
	
	a.get(n, m, arr);
	a.Joseph();
	
	return 0;	
}
