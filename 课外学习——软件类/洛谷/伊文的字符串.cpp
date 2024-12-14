#include <iostream>

using namespace std;

class Even
{
	private:
		int m, n;
		int A[10000], B[10000];
		
	public:
		void get(char* newArr, char* newBrr, int newM, int newN)
		{
			for (int i = 0; i < newM; i++) 
			{  
            	A[i] = newArr[i] - '0';
        	}  
        	
        	for (int i = 0; i < newN; i++) 
			{  
            	B[i] = newBrr[i] - '0';  
        	} 
        	
        	m = newM; 
        	n = newN; 
		}
		
		void XOR()
		{	
			int t = 0;
			int count = 0;
			int si[n];
			
			for (int x = 0; x <= m - n; x++)
			{
				int xorSum[n] = {0};
				
				for (int i = 0; i < n; i++)
				{
					xorSum[i] ^= A[x + i] ^ B[i];
				}
				
				for (int i = 0; i < n; i++)
				{
					if (xorSum[i] == 0)
					{
						t++;
					}
				}
				if (t == n)
				{
					count++; 
				}
			}
			
			cout << count;
		}	
};

int main()
{
	Even game;
	
	int m, n;
	char arr[10000], brr[10000];
	
	cin >> m >> n;
	
	for (int i = 0; i < m; i++)
	{
		cin >> arr[i];
	}
	
	for (int i = 0; i < n; i++)
	{
		cin >> brr[i];
	}
	
	game.get(arr, brr, m, n); 
	game.XOR();
	
	return 0;
}
