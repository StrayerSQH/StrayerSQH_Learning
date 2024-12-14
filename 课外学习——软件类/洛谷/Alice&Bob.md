这里有n个正整数，a1,...,an
Alice 会先去掉其中最多d个数
Bob 接下来会将剩余的数中最多m个数乘以 -k
Alice 想要剩余数之和尽可能大，Bob 想要剩余数之和尽可能小。假设 Alice 和 Bob 都足够聪明，请问最后剩余数之和是多少。
输入描述
第一行一个正整数T，接下来有T组数据
每组数据 2 行
第一行 4 个数 
n,m,k,d(2<=n<=10^5)(0<=m,d<=n)(1<=k<=10^4)
第二行n个数，a1,a2,...,an(1<=ai<=10^9)
保证$\sum n$不超过 10^5
输出描述
每组数据输出一行，每行一个数，表示剩余数之和
示例 1
输入
```
1
3 1 1 1
4 1 1
```
输出
```
0
```
说明
Alice会去掉4，此时剩余数为[1, 1]
Bob会把一个1变为-1，此时剩余数为[-1, 1]，和为0

```
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
```
