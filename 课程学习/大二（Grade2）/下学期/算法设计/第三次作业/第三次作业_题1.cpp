#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> height(100000);

    string str;
    getline(cin, str);

    char t = '[';
    int i = 1;
    int j = 0;
    int num = 0;
    while (true)
    {
        if ('0' <= str[i] && str[i] <= '9')
        {
            num = num * 10 + str[i] - '0';
        }
        else if (str[i] == ']')
        {
            height[j] = num;
            num = 0;
            break;
        }
        else
        {
            height[j] = num;
            num = 0;
            j++;
            i++;
        }

        i++;
    }

    int n = height.size();
    int left = 0;
    int right = n - 1;
    int maxSize = 0;

    while (left < right)
    {
        int current = min(height[left], height[right]) * (right - left);
        maxSize = max(current, maxSize);

        if (height[left] < height[right])
        {
            left++;
        }
        else
        {
            right--;
        }
    }

    cout << maxSize << endl;

    return 0;
}