#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
	string str;
	cin >> str;
	int arr[26];
	for (int i = 0; i < str.size(); i++)
	{
		arr[str[i] - 'a'] = i + 1;
	}
	
	int j = 0;
	int start = 0, end = 0;
	int result[500];

	for (int i = 0; i < str.size(); i++)
	{
		end = max(end, arr[str[i] - 'a']);

		if (i + 1 == end)
		{
			result[j] = end - start;
			start = i + 1;
			j++;
		}
	}

	cout << "[";
	int i = 0;
	for (i = 0; i < j - 1; i++)
	{
		cout << result[i] << ",";
	}
	cout << result[i];
	cout << "]";

	return 0;
}