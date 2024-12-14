#include <iostream>
#include <cstring>

using namespace std;

int main()
{
	char arr[301];
	char brr[11];
	char crr[11];
	
	cin >> arr;
	cin >> brr;
	
	int len1 = strlen(arr);
	int len2 = strlen(brr);
	int min, max;
	int judge1 = 0, judge2 = 0;
	
	for (int i = 0; i < len2; i++)
	{
		crr[len2 - i - 1] = brr[i];
	}
	
	for (int i = 0; i < len1 - len2; i++)
	{
		int tmp = 0;
		for (int j = 0; j < len2; j++)
		{
			if (arr[i + j] == brr[j])
			{
				tmp++;
			}
			else
			{
				tmp = 0;
				break;
			}
		}
		
		if (tmp == len2)
		{
			min = i + len2;
			judge1 = 1;
			break;
		}
	}
	
	for (int i = 0; i < len1; i++)
	{
		int tmp1 = 0;
		for (int j = 0; j < len2; j++)
		{
			if (arr[i + j] == crr[j])
			{
				tmp1++;
			}
			else
			{
				tmp1 = 0;
				break;
			}
		}	
		
		if (tmp1 == len2)
		{
			max = i;
			judge2 = 1;
		}
	}
	
	if (judge1 && judge2)
	{
		cout << max - min;
	}
	else
	{
		cout << "no";
	}
	
	return 0;
}
