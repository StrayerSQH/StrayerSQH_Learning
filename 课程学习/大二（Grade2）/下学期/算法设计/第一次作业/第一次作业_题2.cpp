#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// 计算跨越中间的最大和
int crossSum(vector<int>& v, int l, int m, int r) {
    int lsum = INT_MIN, sum = 0;
    for (int i = m; i >= l; i--) {
        sum += v[i];
        if (sum > lsum) lsum = sum;
    }

    int rsum = INT_MIN;
    sum = 0;
    for (int i = m + 1; i <= r; i++) {
        sum += v[i];
        if (sum > rsum) rsum = sum;
    }

    return lsum + rsum;
}

// 分治递归
int helper(vector<int>& v, int l, int r) {
    if (l == r) return v[l];

    int m = l + (r - l) / 2;
    int left = helper(v, l, m);
    int right = helper(v, m + 1, r);
    int cross = crossSum(v, l, m, r);

    return max({ left, right, cross });
}

// 主求解函数
int maxSubArr(vector<int>& v) {
    return helper(v, 0, v.size() - 1);
}

// 输入解析
vector<int> getNums(string s) {
    vector<int> res;
    size_t start = s.find('[');
    size_t end = s.find(']');
    if (start == string::npos || end == string::npos) return res;

    string arr = s.substr(start + 1, end - start - 1);
    stringstream ss(arr);
    string num;
    while (getline(ss, num, ',')) {
        res.push_back(stoi(num));
    }
    return res;
}

int main() {
    string s;
    getline(cin, s);

    vector<int> v = getNums(s);
    if (v.empty()) {
        cout << "输入格式错误！" << endl;
        return 1;
    }

    cout << maxSubArr(v) << endl;

    return 0;
}