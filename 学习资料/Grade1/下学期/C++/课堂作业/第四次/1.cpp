#include <iostream>
#include <string>
#include <vector>

using namespace std;
using std::vector;

vector<int> add(vector<int> result, int num, int res)
{
	int sum;
	sum = result[num] + res;
	if (sum >= 10)
	{
		int odd = sum % 10;
		int ten = sum / 10;
		result[num] = odd;
		result = add(result, num + 1, ten);	
		
	}
	else
	{
		result[num] = sum;
	}
		
	return result;
}

int main()
{
    int a;
    cin >> a;

    for (int k = 0; k < a; k++)
    {
        string str1;
        string str2;
        char res;
        
        cin >> str1 >> str2;
        
        long long len1 = str1.size();
        long long len2 = str2.size();
        long long n = len1 + len2;
        
        vector<int> result(n);
        
        for (int i = len1 - 1; i >= 0; i--)
        {
        	for (int j = len2 - 1; j >= 0; j--)
        	{
        		int num = (len1 - 1 - i) + (len2 - 1 - j);
        		int num1 = str1[i] - '0';
        		int num2 = str2[j] - '0';
        		int product = num1 * num2;
        		result = add(result, num, product);
			}
		}
		
		if (result[n - 1] == 0)
		{
			n--;
		}
		
		for (int i = 0; i < n; i++)
		{
			res = result[n - i - 1] + '0';
			cout << res;
		}
		cout << endl;
    }
    
    return 0;
}
