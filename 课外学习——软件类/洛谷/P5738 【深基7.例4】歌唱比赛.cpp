#include <iostream>
#include <cstdio>

using namespace std;

void bubblesort(double arr[], double m)
{
	for (int i = 0; i < m - 1; i++)
	{
		for (int j = 0; j < m - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				double tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

double average(double dafen[], double m)
{
	double sum = 0;
	
	for (int i = 1; i < m - 1; i++)
	{
		sum += dafen[i];
	}
	
	double grade = sum / (m - 2);
	
	return grade;
}

int main()
{
	int n, m;
	double final[10000];
	int k = 0;
	
	cin >> n >> m;
	
	for (int i = 0; i < n; i++)
	{
		double dafen[m];
		
		for (int j = 0; j < m; j++)
		{
			cin >> dafen[j];
		}
		
		bubblesort(dafen, m);
		final[k] = average(dafen, m);
		k++;
	}
	
	bubblesort(final, n);
	
	printf("%.2lf", final[n - 1]);
	
	return 0;
}
