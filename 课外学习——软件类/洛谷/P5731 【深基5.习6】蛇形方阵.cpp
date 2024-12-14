#include <cstdio>

using namespace std;

int main()
{
	int n;
	int arr[100][100];
	int s = 1;
	int a = 1;
	
	scanf("%d", &n);
	
	int shang = 0, xia = n - 1, zuo = 0, you = n -1;

	while (n * n - a + 1)
	{
		if (s == 1)
		{
			for (int i = zuo; i <= you; i++)
			{
				arr[shang][i] = a;
				a++;
			}
			s = 2;
			shang++;
		}
	
		if (s == 2)
		{
			for (int i = shang; i <= xia; i++)
			{
				arr[i][you] = a;
				a++;
			}
			s = 3;
			you--;
		}
	
		if (s == 3)
		{
			for (int i = you; i >= zuo; i--)
			{
				arr[xia][i] = a;
				a++;
			}
			s = 4;
			xia--;
		}
		if (s ==4)
		{
			for (int i = xia; i >= shang; i--)
			{
				arr[i][zuo] = a;
				a++;
			}
			s = 1;
			zuo++;
		}
	}
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%4d", arr[i][j]);
		}
		printf("\n");
	}
	
	return 0;
}
