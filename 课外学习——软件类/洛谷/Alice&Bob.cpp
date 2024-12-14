#include <iostream>

using namespace std;

class Alice_Bob
{
	private:
		int arr[100000];
		int n, m, k, d;
		int sum = 0;
	
	public:
		void get(int* newArr, int newN, int newM, int newK, int newD)
		{
			for (int i = 0; i < newN; i++) 
			{  
            	arr[i] = newArr[i];  
        	}  
        	n = newN;  
        	m = newM;  
        	k = newK;  
        	d = newD;
		}
		
		void BubbleSort()
		{
			for (int i = 0; i < n - 1; i++) 
			{  
        		for (int j = 0; j < n - i - 1; j++) 
				{  
            		if (arr[j] < arr[j + 1]) 
					{  
                		int temp = arr[j];  
                		arr[j] = arr[j + 1];  
                		arr[j + 1] = temp;  
            		}  
        		}  
    		}  
		}
		
		void Alice()
		{
			for (int i = 0; i < d; i++)
			{
				arr[i] = 0;
			}
		}
		
		void Bob()
		{
			for (int i = d; i < d + m; i++)
			{
				arr[i] *= -k;
			}
		}
		 
		void result()
		{
			for (int i = 0; i < n; i++)
			{
				sum += arr[i];
			}
			
			cout << sum;
		}
}; 

int main()
{
	int t;
	cin >> t;
	
	for (int i = 0; i < t; i++)
	{
		Alice_Bob AB;
		
		int n, m, k, d;
		int arr[100000];
		cin >> n >> m >> k >> d;	
		
		for (int i = 0; i < n; i++)
		{
			cin >> arr[i];
		}
		
		AB.get(arr, n, m, k, d);
		AB.BubbleSort();
		AB.Alice();
		AB.Bob();
		AB.result();
	} 
	
	return 0;
} 
