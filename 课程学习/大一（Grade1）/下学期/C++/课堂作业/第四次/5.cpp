#include <iostream>

using namespace std;

void operation(int arr[20], int brr[20], int n, int len)
{
	if (n == len)
    {
        cout << "-->";
        for (int i = 0; i < len; i++)
        {
            if (brr[i] == 1)
                cout << " " << arr[i];
        }

        cout << endl;
        return;
    }
    
    brr[n] = 1;
    operation(arr, brr, n + 1, len);

    brr[n] = 0;
    operation(arr, brr, n + 1, len);
}

int main()
{
	int n;
	cin >> n;
	
	for (int i = 0; i < n; i++)
	{
		int t;
		cin >> t;
		
		int arr[t];
		int brr[t];
		
		for (int j = 0; j < t; j++)
		{
			cin >> arr[j];
		} 
		
		operation(arr, brr, 0, t);
	}
}
