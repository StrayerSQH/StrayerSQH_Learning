#include <iostream>
#include <vector>

using namespace std;

int MaxProfit(vector<int>& nums)
{
	int n = nums.size();
	int minPrice = nums[0];
	int maxProfit = 0;

	if (n <= 1)
	{
		return 0; //如果输入的值无法满足计算的条件直接返回
	}

	for (int i = 0; i < n; i++)
	{
		if (nums[i] < minPrice)
		{
			minPrice = nums[i]; //更新最小值
		}
		else 
		{
			int currentProfit = nums[i] - minPrice; //计算当前的利润
			if (currentProfit > maxProfit) 
			{
				//如果当前的利润大于之前计算出的最大的利润则重新更新最大的利润
				maxProfit = currentProfit; 
			}
		}
	}

	return maxProfit; //返回最大值
}

int main()
{
	vector<int> num1 = { 2, 1, 4, 8, 3 };
	vector<int> num2 = { 3, 4, 5, 1, 2, 6, 6, 7 };

	cout << MaxProfit(num1) << endl;
	cout << MaxProfit(num2) << endl;
}