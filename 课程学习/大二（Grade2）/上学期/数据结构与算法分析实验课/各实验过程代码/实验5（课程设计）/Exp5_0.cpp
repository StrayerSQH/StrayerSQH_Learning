#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <tuple>
#include <algorithm>
#include <cctype> // 包含towupper函数的头文件

int SCREEN_WIDTH = static_cast<int>(sf::VideoMode::getDesktopMode().width);
int SCREEN_HEIGHT = static_cast<int>(sf::VideoMode::getDesktopMode().height);
char END;
int FRONT, BACK, DISTANCE;
int POINTS_NUMBER;

using namespace std;

void Introduction() {
    string name = "SQH";
    string studentID = "2023141460***";

    cout << "+------------------------------------------------------+" << endl;
    cout << "| 姓名：" << name;
    cout << string(46 - name.length(), ' ') << " |" << endl;
    cout << "| 学号：" << studentID;
    cout << string(46 - studentID.length(), ' ') << " |" << endl;
    cout << "+------------------------------------------------------+" << endl;

    for (int i = 0; i < 2; i++) {
        cout << endl;
    }
}

sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), "SFML works!");
sf::Font font;
sf::CircleShape circles[8];
sf::Text texts[8];
struct Vertex {
    sf::Vertex vertex;
    int front;
    int back;
    int distance;
};

Vertex vertexs[100];
sf::VertexArray edges; // 用于存储边的顶点数组

class SFML {
private:
    int vertextsId = 0;
public:
    SFML() {};

    void CreatWindows() {
        // 创建窗口
        window.setPosition(sf::Vector2i(0, 0));
    }

    void FrontLoad() {
        if (!font.loadFromFile("C:/Windows/Fonts/simsun.ttc")) { // 确保路径正确
            cerr << "字体加载失败，请检查!" << endl;
            exit(3);
        }
    }

    void CreatPoint(int POINTS_NUMBER) {
        // 创建节点
        float radius = 250;
        float angleStep = 2 * 3.14159265f / POINTS_NUMBER;

        for (int i = 0; i < POINTS_NUMBER; ++i) {
            circles[i].setRadius(20.f);
            circles[i].setFillColor(sf::Color::White);
            circles[i].setOutlineThickness(5.f);
            circles[i].setOutlineColor(sf::Color::Black);

            float angle = i * angleStep;
            float x = SCREEN_WIDTH / 4 + radius * cos(angle) - 25 - 100;
            float y = SCREEN_HEIGHT / 4 + radius * sin(angle) - 25;

            circles[i].setPosition(x, y);

            texts[i].setFont(font);
            texts[i].setCharacterSize(40);
            texts[i].setFillColor(sf::Color::Black);
            texts[i].setString(to_string(i));
            texts[i].setStyle(sf::Text::Bold);
            sf::FloatRect textRect = texts[i].getLocalBounds();
            texts[i].setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);
            texts[i].setPosition(circles[i].getPosition().x + 20, circles[i].getPosition().y + 20);
        }
    }

    int GetPoint_x(int point_Id) {
        return circles[point_Id].getPosition().x;
    }

    int GetPoint_y(int point_Id) {
        return circles[point_Id].getPosition().y;
    }

    void AddVertex(int front, int back, int distance) {
        // 将边的两个端点添加到edges数组中
        edges.append(sf::Vertex(sf::Vector2f(GetPoint_x(front) + 20, GetPoint_y(front) + 20), sf::Color::White));
        edges.append(sf::Vertex(sf::Vector2f(GetPoint_x(back) + 20, GetPoint_y(back) + 20), sf::Color::White));
        if (edges.getVertexCount() >= 2) {
            edges.setPrimitiveType(sf::Lines); // 设置顶点数组类型为线条
        }
        vertextsId++;
    }

    void DrawEdges() {
        window.draw(edges);
    }
};

class Graph {
private:
    int MAX_VERTICES = 8;
    int vexNum; // 顶点数
    int arcs[8][8]; // 邻接矩阵

public:
    // 构造函数
    Graph(int vertices) : vexNum(vertices) {
        for (int i = 0; i < MAX_VERTICES; ++i) {
            for (int j = 0; j < MAX_VERTICES; ++j) {
                arcs[i][j] = INT_MAX; // 初始化为无边
            }
        }
        for (int i = 0; i < vertices; ++i) {
            arcs[i][i] = 0; // 对角线元素设置为0
        }
    }

    // 添加边
    void addEdge(int src, int dest, int weight) {
        arcs[src - 1][dest - 1] = weight;
        arcs[dest - 1][src - 1] = weight; // 无向图的对称性
    }

    // Prim算法求最小生成树
    int prim() {
        int minCost = 0;
        int minWeight = INT_MAX;
        bool selected[8] = { false };
        int minCostIdx[8];

        for (int i = 0; i < vexNum; ++i) {
            minCostIdx[i] = INT_MAX;
        }
        minCostIdx[0] = 0; // 从顶点0开始

        for (int i = 0; i < vexNum; ++i) {
            int u = -1, minDistance = INT_MAX;
            for (int j = 0; j < vexNum; ++j) {
                if (!selected[j] && minDistance > arcs[j][0]) {
                    u = j;
                    minDistance = arcs[j][0];
                }
            }

            if (u == -1) break;

            selected[u] = true;
            minCost += minDistance;

            for (int v = 0; v < vexNum; ++v) {
                if (!selected[v] && arcs[u][v] < minCostIdx[v]) {
                    minCostIdx[v] = arcs[u][v];
                }
            }
        }

        return minCost;
    }
};

int main() {
    Introduction();

    SFML sfml;
    sfml.FrontLoad();
    Graph g(POINTS_NUMBER);

    while (true) {
        cout << "请输入需要创建的图的节点数（不超过8个）:";
        cin >> POINTS_NUMBER;
        if (POINTS_NUMBER > 8 || POINTS_NUMBER < 1) {
            cout << "输入的节点数不符合要求，请输入1到8之间的数字。" << endl;
        }
        else {
            sfml.CreatPoint(POINTS_NUMBER);
            break;
        }
    }

    while (true) {
        cout << "是否终止输入（T/F)： ";
        cin >> END;
        END = toupper(END); // 使用toupper函数
        if (END == 'T') {
            break;
        }
        else {
            cout << "输入节点之间的关系，格式为【前节点 后节点 距离】：";
            cin >> FRONT >> BACK >> DISTANCE;
            g.addEdge(FRONT, BACK, DISTANCE);
            sfml.AddVertex(FRONT, BACK, DISTANCE);
        }
    }

   
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        sfml.DrawEdges(); // 绘制边
        for (int i = 0; i < POINTS_NUMBER; ++i) {
            window.draw(circles[i]);
            window.draw(texts[i]);
        }


        window.display();
    }

    return 0;
}
