#include <iostream>
#include <cmath>
#include <cstdio>

using namespace std;

double dis(double x1, double y1, double x2, double y2)
{
	double a = sqrt(abs((x1 - x2) * (x1 - x2)) + abs((y1 - y2) * (y1 - y2)));
	
	return a;
}

int main()
{
	double arr[6];
	double sum = 0;
	
	for (int i = 0; i < 6; i++)
	{
		cin >> arr[i];
	}
	
	sum += dis(arr[0], arr[1], arr[2], arr[3]);
	sum += dis(arr[0], arr[1], arr[4], arr[5]);
	sum += dis(arr[4], arr[5], arr[2], arr[3]);
	
	printf("%.2f", sum);
	return 0;
} 
