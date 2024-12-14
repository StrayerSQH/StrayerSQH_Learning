#include <iostream>
#include <string>
#include <vector>

using namespace std;
using std::vector;

vector<int> add(vector<int> result, long long num, int t)
{
	int sum;
	sum = result[num] + t;
	
	if (sum >= 10)
	{
		int a = sum % 10;
		int b = sum / 10;
		result[num] = a;
		result = add(result, num + 1, b);
	}
	else
	{
		result[num] = sum;
	}
	
	return result;
}

string shift(string t, int len)
{
	for (int i = 0; i < len ; i++)
	{
		t[i] = t[i + 1];
	}
	
	return t;
}

int main()
{
	int n;
	cin >> n;
	
	for (int i = 0; i < n; i++)
	{
		string a, b;
		cin >> a >> b;
		long long len1 = a.size();
		long long len2 = b.size();
		int state;
		char res;
		
		if (a[0] == '0' || b[0] == '0')
		{
			cout << "0" << endl;
			
			continue;	
		} 
		
		if (a[0] == '-' && b[0] == '-')
		{
			state = 2;
			
			a = shift(a, len1);
			b = shift(b, len2);
			
			len1--;
			len2--;
		}
		else if ((a[0] == '-' && b[0] != '-') || (a[0] != '-' && b[0] == '-'))
		{
			state = 1;
			
			if (a[0] == '-' && b[0] != '-')
			{
				a = shift(a, len1);
				len1--;
			}
			else
			{
				b = shift(b, len2);
				len2--;
			}
		}
		else
		{
			state = 0;
		}
		
		vector<int> result(len1 + len2);
		
		for (long long j = len1 - 1; j >= 0; j--)
		{
			for (long long k = len2 - 1; k >= 0; k--)
			{
				long long num = (len1 - j - 1) + (len2 - k - 1);
				int num1 = a[j] - '0';
				int num2 = b[k] - '0';
				int product = num1 * num2;
				
				result = add(result, num, product);
			}
		}
		
		if (result[len1 + len2 - 1] == 0)
		{
			len1 + len2--;
		}
		
		if (state == 1)
		{
			cout << "-";
		}
		
		for (long long j = 0; j < len1 + len2; j++)
		{
			res = result[len1 + len2 - j - 1] + '0';
			cout << res;
		}
		cout << endl;
	}
	
	return 0;
}
