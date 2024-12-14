#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

class Solution {
public:
    void solve() {
        int T;
        cin >> T;

        for (int t = 0; t < T; t++) {
            string s;
            cin >> s;

            if (s.empty()) {
                cout << 0 << endl;
                continue;
            }

            int n = s.length();

            if (n == 1) {
                cout << 1 << endl;
                continue;
            }

            vector<vector<int> > dp(n, vector<int>(n, 1000));

            for (int i = 0; i < n; i++) {
                dp[i][i] = 1;
            }
			
			int i, j, k;
            for (j = 1; j < n; j++) {
                for (i = j - 1; i >= 0; i--) {
                    if (i == j - 1) {
                        dp[i][j] = (s[i] == s[j]) ? 1 : 2;
                        continue;
                    }

                    if (s[i] == s[j]) {
                        dp[i][j] = min(dp[i][j], dp[i + 1][j - 1]);
                    }

                    for (k = i; k < j; k++) {
                        dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j]);
                    }
                    printf("%d %d %d\n", i, j, k); 
                }
                
                for (int i = 0; i < n; i++)
                {
                	for (int k = 0; k < n; k++)
                	{
                		printf("%4d ", dp[i][k]);
					}
					cout << endl;
				}
            }

            cout << dp[0][n - 1] << endl;
        }
    }
};

int main() {
    Solution solution;
    solution.solve();

    return 0;
}
