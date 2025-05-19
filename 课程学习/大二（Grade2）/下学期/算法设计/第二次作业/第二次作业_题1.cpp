#include <iostream>
#include <vector>
using namespace std;

bool canJump(vector<int>& nums) {
    int max_reach = 0;  //存储可以到达的最远位置
    int n = nums.size(); 

    for (int i = 0; i < n; ++i) {
        if (i > max_reach) {
            return false; // 超出数组则不对
        }
        //更新max_reach
        max_reach = max(max_reach, i + nums[i]);
        if (max_reach >= n - 1) {
            return true;  //如果到达了最后一个则返回true，表明正确的找到了最后一个
        }
    }

    return false;  //默认返回作物
}

int main() {
    vector<int> nums1 = { 3, 4, 1, 0, 2, 1 };
    vector<int> nums2 = { 3, 0, 4, 1, 2, 0, 0, 2 };

    if (canJump(nums1))
    {
        cout << "true" << endl;
    }
    else
    {
        cout << "false" << endl;
    }

    if (canJump(nums2))
    {
        cout << "true" << endl;
    }
    else
    {
        cout << "false" << endl;
    }

    return 0;
}