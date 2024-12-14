#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int countSuperCourierPoints(int n, int m, vector<vector<int> >& edges) {
    vector<vector<int> > graph(n + 1);  // 创建邻接表表示的有向图
    vector<int> inDegree(n + 1, 0);    // 记录每个节点的入度

    // 构建有向图和入度数组
    for (const auto& edge : edges) {
        int u = edge[0], v = edge[1];
        graph[u].push_back(v);
        inDegree[v]++;
    }

    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (inDegree[i] == 0)
            q.push(i);  // 将入度为0的节点入队
    }

    int superCourierPoints = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        bool isSuperCourierPoint = true;  // 是否为超级快递点

        for (int v : graph[u]) {
            if (--inDegree[v] == 0)
                q.push(v);  // 将入度减为0的节点入队
            else
                isSuperCourierPoint = false;  // 存在无法到达的节点，不是超级快递点
        }

        if (isSuperCourierPoint)
            superCourierPoints++;
    }

    return superCourierPoints;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> edges(m, vector<int>(2));
    for (int i = 0; i < m; i++) {
        cin >> edges[i][0] >> edges[i][1];
    }

    int result = countSuperCourierPoints(n, m, edges);
    cout << result << endl;

    return 0;
}
