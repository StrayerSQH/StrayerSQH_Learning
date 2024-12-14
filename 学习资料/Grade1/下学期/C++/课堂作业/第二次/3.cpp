#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int countSuperCourierPoints(int n, int m, vector<vector<int> >& edges) {
    vector<vector<int> > graph(n + 1);  // �����ڽӱ��ʾ������ͼ
    vector<int> inDegree(n + 1, 0);    // ��¼ÿ���ڵ�����

    // ��������ͼ���������
    for (const auto& edge : edges) {
        int u = edge[0], v = edge[1];
        graph[u].push_back(v);
        inDegree[v]++;
    }

    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (inDegree[i] == 0)
            q.push(i);  // �����Ϊ0�Ľڵ����
    }

    int superCourierPoints = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        bool isSuperCourierPoint = true;  // �Ƿ�Ϊ������ݵ�

        for (int v : graph[u]) {
            if (--inDegree[v] == 0)
                q.push(v);  // ����ȼ�Ϊ0�Ľڵ����
            else
                isSuperCourierPoint = false;  // �����޷�����Ľڵ㣬���ǳ�����ݵ�
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
