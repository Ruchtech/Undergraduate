#pragma once

#include"head.h"
#include"Vertex.h"


// 图类
class Graph {
private:
    int vexnum; // 顶点数量
    int adjMatrix[MVNum][MVNum]; // 邻接矩阵
    bool visi[MVNum];

public:
    // 类初始化
    Graph(int vexnum) : vexnum(vexnum) {
        // 初始化邻接矩阵
        for (int i = 0; i < vexnum; ++i) {
            for (int j = 0; j < vexnum; ++j) {
                adjMatrix[i][j] = 0;
            }
            visi[MVNum] = false;
        }
    }

    // 类元素构建--添加边
    void addEdge(int src, int dest, int weight) {
        adjMatrix[src][dest] = weight;
        adjMatrix[dest][src] = weight;
    }

    // Dijkstra算法实现
    void Dijkstra(int src, int* prev, int* dist) {
        bool visited[MVNum] = { false };

        // 初始化
        for (int i = 0; i < vexnum; ++i) {
            prev[i] = -1;
            dist[i] = INF;        //为什么，因为一次只取一个，只求最短最多结点的路径，而不是所有结点最短路径
        }

        dist[src] = 0;

        for (int count = 0; count < vexnum - 1; ++count) {
            int u = minDistance(dist, visited);
            visited[u] = true;

            //找u最近的v点
            for (int v = 0; v < vexnum; ++v) {
                if (!visited[v] && dist[u] + adjMatrix[u][v] < dist[v]) {
                //if (!visited[v] && adjMatrix[u][v] && dist[u] != INF && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    prev[v] = u;
                }
            }
        }
    }
    // 寻找距离最近的顶点
    int minDistance(int* dist, bool* visited) {
        int min = INF, min_index = -1;

        for (int v = 0; v < vexnum; ++v) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                min_index = v;
            }
        }

        return min_index;
    }
    // 检查所有顶点是否都被访问过
    bool allVisited(int* vis) {
        for (int i = 0; i < vexnum; ++i) {
            if (vis[i] == 0) {
                return false;
            }
        }
        return true;
    }
    // 输出最短路径
    void printShortestPathsFromSource(int begin, int* prev, int* dist, string name[]) {
        // 使用vector存储目的地索引和对应的距离
        vector<pair<int, int>> destinations;

        // 将目的地索引和对应的距离存储到vector中
        for (int i = 0; i < vexnum; ++i) {
            if (i != begin && dist[i] < INF) {
                destinations.push_back(make_pair(i, dist[i]));
            }
        }

        // 按照距离从小到大对目的地进行排序
        sort(destinations.begin(), destinations.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second < b.second;
            });

        // 输出路径
        for (const auto& dest : destinations) {
            int destination = dest.first;
            int distance = dest.second;
            cout << "                         目的地: " << name[destination] << ", 路径长度: " << distance << ", 路径: ";
            printShortestPaths(begin, destination, prev, MVNum, name);
            cout << endl;
        }
    }
    void printShortestPaths(int source, int destination, int Pr[], int size, string name[]) {
        if (destination == source) {
            cout << name[source];
            return;
        }
        printShortestPaths(source, Pr[destination], Pr, size, name);
        cout << " -> " << name[destination];
    }


    // DFS_异常处理_连通判断
    bool DFS(int v)
    {
        visi[v] = true;
        for (int i = 0; i < vexnum; i++)
        {
            int n = adjMatrix[v][i];
            if ((adjMatrix[v][i] < INF) && !visi[i]) {
                DFS(i);
            }
        }
        return true;
    }

    // 最小生成树
    void mintree(string name[]) {

        for (int i = 0; i < vexnum; i++) {
            visi[i] = false;
        }
        DFS(0);
        for (int i = 0; i < vexnum; i++) {
            if (!visi[i]) {
                system("cls");
                cout << "                         无法连通所有路径!!!!!" << endl;
                return;
            }
        }


        // Prim 算法

        int lowcost[MVNum]; // 存放当前节点到最小生成树的最短距离
        int closest[MVNum]; // 存放当前节点在最小生成树中的邻接节点
        bool s[MVNum];      // 标记节点是否已加入最小生成树

        // 初始化数组
        for (int i = 0; i < vexnum; i++) {
            lowcost[i] = adjMatrix[0][i];  // 从节点0开始构建最小生成树
            closest[i] = 0;
            s[i] = false;
        }

        s[0] = true;  // 将节点0标记为已加入最小生成树

        cout << "                         铺设电路规划：" << endl;

        int totalLength = 0; // 总长度

        // 逐步构建最小生成树
        for (int i = 1; i < vexnum; i++) {
            int minIndex = -1;  // 记录当前最小距离的节点索引
            int minDistance = INF; // 初始化最小距离为无穷大

            // 贪心算法  找到当前最小距离的节点
            for (int j = 0; j < vexnum; j++) {
                if (!s[j] && lowcost[j] < minDistance) {
                    minIndex = j;
                    minDistance = lowcost[j];
                }
            }


            // 输出电路信息
            cout << "                         " << name[closest[minIndex]] << " -- " << name[minIndex] << "  长度：" << minDistance << endl;

            totalLength += minDistance;

            s[minIndex] = true; // 将最小距离节点标记为已加入最小生成树

            // 更新最小距离数组和最小距离节点的邻接节点信息
            for (int j = 0; j < vexnum; j++) {
                if (!s[j] && adjMatrix[minIndex][j] < lowcost[j]) {
                    lowcost[j] = adjMatrix[minIndex][j];
                    closest[j] = minIndex;
                }
            }
        }

        cout << "                         总长度：" << totalLength << endl;
    }

    // 输出推荐路径1
    void print_recommended_path_1(Vertex v, string na[]) {

        // 连通判断
        for (int i = 0; i < vexnum; i++) {
            visi[i] = false;
        }
        DFS(v.ID);
        for (int i = 0; i < vexnum; i++) {
            if (!visi[i]) {
                system("cls");
                cout << "                         无法连通所有路径!!!!!" << endl;
                return;
            }
        }


        // 异常处理
        if (v.ID < 0 || v.ID >= vexnum) {
            cout << "                         无效的顶点序号" << endl;
            return;
        }

        int vis[MVNum] = { 0 }; // 标记顶点是否访问过
        vis[v.ID] = 1;

        vector<int> path_list;
        path_list.push_back(v.ID);//存储最短路径的顶点序列

        int totalLength = 0;

        while (!allVisited(vis)) {
            int prev[MVNum] = { 0 };
            int dist[MVNum] = { 0 };
            int next_vex = -1;

            // 寻找下一个未访问的顶点
            for (int j = 0; j < vexnum; ++j) {
                if (!vis[j]) {
                    next_vex = j;
                    break;
                }
            }

            //贪心算法
            //计算路径末顶点到其他所有顶点最短距离
            Dijkstra(path_list.back(), prev, dist);

            // 找到下一个顶点
            for (int j = 0; j < vexnum; ++j) {
                if (!vis[j]) {
                    next_vex = (next_vex == -1 || dist[j] < dist[next_vex]) ? j : next_vex;
                }
            }

            //为什么可以重复，因为前驱的应用    +   仅输出为查找的
            //为什么可以可以迪杰斯特拉

            // 更新路径
            vector<int> temp;//可重复体现在这，及用容器添加，而不是单独一个，因为可重复
            for (int j = next_vex; j != path_list.back(); j = prev[j]) {
                temp.insert(temp.begin(), j);                 //将顶点j添加到temp容器的开头
            }
            path_list.insert(path_list.end(), temp.begin(), temp.end());

            totalLength += dist[next_vex];
            vis[next_vex] = 1;

        }


        // 输出路径
        cout << "推荐路径起始点 \"" << v.name << "\": ";
        for (int i = 0; i < path_list.size() - 1; ++i) {
            cout << na[path_list[i]] << " -> ";
        }
        cout << na[path_list.back()] << endl;

        //输出路径长度
        cout << "总距离: " << totalLength << endl;

    }

    // 输出推荐路径2
    void print_recommended_path_2(Vertex v, string na[]) {

        for (int i = 0; i < vexnum; i++) {
            visi[i] = false;
        }
        DFS(v.ID);
        for (int i = 0; i < vexnum; i++) {
            if (!visi[i]) {
                system("cls");
                cout << "                         无法连通所有路径!!!!!" << endl;
                return;
            }
        }


        if (v.ID < 0 || v.ID >= vexnum) {
            cout << "Invalid vertex ID" << endl;
            return;
        }

        int vis[MVNum] = { 0 }; // 标记顶点是否访问过
        vis[v.ID] = 1;

        vector<int> path_list;
        path_list.push_back(v.ID);

        int totalLength = 0;

        while (!allVisited(vis)) {
            int prev[MVNum] = { 0 };
            int dist[MVNum] = { 0 };
            int next_vex = -1;

            // 寻找下一个未访问的顶点
            for (int j = 0; j < vexnum; ++j) {
                if (!vis[j]) {
                    next_vex = j;
                    break;
                }
            }

            Dijkstra(path_list.back(), prev, dist);

            // 找到下一个顶点
            for (int j = 0; j < vexnum; ++j) {
                if (!vis[j]) {
                    next_vex = (next_vex == -1 || dist[j] < dist[next_vex]) ? j : next_vex;
                }
            }

            // 更新路径
            vector<int> temp;
            for (int j = next_vex; j != path_list.back(); j = prev[j]) {
                temp.insert(temp.begin(), j);
            }
            path_list.insert(path_list.end(), temp.begin(), temp.end());

            totalLength += dist[next_vex];
            vis[next_vex] = 1;
        }



        // 添加回到起点的路径
        int prev[MVNum] = { 0 };
        int dist[MVNum] = { 0 };
        Dijkstra(path_list.back(), prev, dist);
        vector<int> temp;
        for (int j = v.ID; j != path_list.back(); j = prev[j]) {
            temp.insert(temp.begin(), j);
        }
        path_list.insert(path_list.end(), temp.begin(), temp.end());

        totalLength += dist[v.ID]; // 回到起点的距离
        vis[v.ID] = 1;


        // 输出路径
        cout << "推荐路径开始与结束 \"" << v.name << "\": ";
        for (int i = 0; i < path_list.size() - 1; ++i) {
            cout << na[path_list[i]] << " -> ";
        }
        cout << na[path_list.back()] << endl;
        cout << "总距离: " << totalLength << endl;


    }

};
