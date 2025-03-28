#pragma once

#include"head.h"
#include"Vertex.h"


// ͼ��
class Graph {
private:
    int vexnum; // ��������
    int adjMatrix[MVNum][MVNum]; // �ڽӾ���
    bool visi[MVNum];

public:
    // ���ʼ��
    Graph(int vexnum) : vexnum(vexnum) {
        // ��ʼ���ڽӾ���
        for (int i = 0; i < vexnum; ++i) {
            for (int j = 0; j < vexnum; ++j) {
                adjMatrix[i][j] = 0;
            }
            visi[MVNum] = false;
        }
    }

    // ��Ԫ�ع���--��ӱ�
    void addEdge(int src, int dest, int weight) {
        adjMatrix[src][dest] = weight;
        adjMatrix[dest][src] = weight;
    }

    // Dijkstra�㷨ʵ��
    void Dijkstra(int src, int* prev, int* dist) {
        bool visited[MVNum] = { false };

        // ��ʼ��
        for (int i = 0; i < vexnum; ++i) {
            prev[i] = -1;
            dist[i] = INF;        //Ϊʲô����Ϊһ��ֻȡһ����ֻ�����������·�������������н�����·��
        }

        dist[src] = 0;

        for (int count = 0; count < vexnum - 1; ++count) {
            int u = minDistance(dist, visited);
            visited[u] = true;

            //��u�����v��
            for (int v = 0; v < vexnum; ++v) {
                if (!visited[v] && dist[u] + adjMatrix[u][v] < dist[v]) {
                //if (!visited[v] && adjMatrix[u][v] && dist[u] != INF && dist[u] + adjMatrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    prev[v] = u;
                }
            }
        }
    }
    // Ѱ�Ҿ�������Ķ���
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
    // ������ж����Ƿ񶼱����ʹ�
    bool allVisited(int* vis) {
        for (int i = 0; i < vexnum; ++i) {
            if (vis[i] == 0) {
                return false;
            }
        }
        return true;
    }
    // ������·��
    void printShortestPathsFromSource(int begin, int* prev, int* dist, string name[]) {
        // ʹ��vector�洢Ŀ�ĵ������Ͷ�Ӧ�ľ���
        vector<pair<int, int>> destinations;

        // ��Ŀ�ĵ������Ͷ�Ӧ�ľ���洢��vector��
        for (int i = 0; i < vexnum; ++i) {
            if (i != begin && dist[i] < INF) {
                destinations.push_back(make_pair(i, dist[i]));
            }
        }

        // ���վ����С�����Ŀ�ĵؽ�������
        sort(destinations.begin(), destinations.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second < b.second;
            });

        // ���·��
        for (const auto& dest : destinations) {
            int destination = dest.first;
            int distance = dest.second;
            cout << "                         Ŀ�ĵ�: " << name[destination] << ", ·������: " << distance << ", ·��: ";
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


    // DFS_�쳣����_��ͨ�ж�
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

    // ��С������
    void mintree(string name[]) {

        for (int i = 0; i < vexnum; i++) {
            visi[i] = false;
        }
        DFS(0);
        for (int i = 0; i < vexnum; i++) {
            if (!visi[i]) {
                system("cls");
                cout << "                         �޷���ͨ����·��!!!!!" << endl;
                return;
            }
        }


        // Prim �㷨

        int lowcost[MVNum]; // ��ŵ�ǰ�ڵ㵽��С����������̾���
        int closest[MVNum]; // ��ŵ�ǰ�ڵ�����С�������е��ڽӽڵ�
        bool s[MVNum];      // ��ǽڵ��Ƿ��Ѽ�����С������

        // ��ʼ������
        for (int i = 0; i < vexnum; i++) {
            lowcost[i] = adjMatrix[0][i];  // �ӽڵ�0��ʼ������С������
            closest[i] = 0;
            s[i] = false;
        }

        s[0] = true;  // ���ڵ�0���Ϊ�Ѽ�����С������

        cout << "                         �����·�滮��" << endl;

        int totalLength = 0; // �ܳ���

        // �𲽹�����С������
        for (int i = 1; i < vexnum; i++) {
            int minIndex = -1;  // ��¼��ǰ��С����Ľڵ�����
            int minDistance = INF; // ��ʼ����С����Ϊ�����

            // ̰���㷨  �ҵ���ǰ��С����Ľڵ�
            for (int j = 0; j < vexnum; j++) {
                if (!s[j] && lowcost[j] < minDistance) {
                    minIndex = j;
                    minDistance = lowcost[j];
                }
            }


            // �����·��Ϣ
            cout << "                         " << name[closest[minIndex]] << " -- " << name[minIndex] << "  ���ȣ�" << minDistance << endl;

            totalLength += minDistance;

            s[minIndex] = true; // ����С����ڵ���Ϊ�Ѽ�����С������

            // ������С�����������С����ڵ���ڽӽڵ���Ϣ
            for (int j = 0; j < vexnum; j++) {
                if (!s[j] && adjMatrix[minIndex][j] < lowcost[j]) {
                    lowcost[j] = adjMatrix[minIndex][j];
                    closest[j] = minIndex;
                }
            }
        }

        cout << "                         �ܳ��ȣ�" << totalLength << endl;
    }

    // ����Ƽ�·��1
    void print_recommended_path_1(Vertex v, string na[]) {

        // ��ͨ�ж�
        for (int i = 0; i < vexnum; i++) {
            visi[i] = false;
        }
        DFS(v.ID);
        for (int i = 0; i < vexnum; i++) {
            if (!visi[i]) {
                system("cls");
                cout << "                         �޷���ͨ����·��!!!!!" << endl;
                return;
            }
        }


        // �쳣����
        if (v.ID < 0 || v.ID >= vexnum) {
            cout << "                         ��Ч�Ķ������" << endl;
            return;
        }

        int vis[MVNum] = { 0 }; // ��Ƕ����Ƿ���ʹ�
        vis[v.ID] = 1;

        vector<int> path_list;
        path_list.push_back(v.ID);//�洢���·���Ķ�������

        int totalLength = 0;

        while (!allVisited(vis)) {
            int prev[MVNum] = { 0 };
            int dist[MVNum] = { 0 };
            int next_vex = -1;

            // Ѱ����һ��δ���ʵĶ���
            for (int j = 0; j < vexnum; ++j) {
                if (!vis[j]) {
                    next_vex = j;
                    break;
                }
            }

            //̰���㷨
            //����·��ĩ���㵽�������ж�����̾���
            Dijkstra(path_list.back(), prev, dist);

            // �ҵ���һ������
            for (int j = 0; j < vexnum; ++j) {
                if (!vis[j]) {
                    next_vex = (next_vex == -1 || dist[j] < dist[next_vex]) ? j : next_vex;
                }
            }

            //Ϊʲô�����ظ�����Ϊǰ����Ӧ��    +   �����Ϊ���ҵ�
            //Ϊʲô���Կ��ԵϽ�˹����

            // ����·��
            vector<int> temp;//���ظ��������⣬����������ӣ������ǵ���һ������Ϊ���ظ�
            for (int j = next_vex; j != path_list.back(); j = prev[j]) {
                temp.insert(temp.begin(), j);                 //������j��ӵ�temp�����Ŀ�ͷ
            }
            path_list.insert(path_list.end(), temp.begin(), temp.end());

            totalLength += dist[next_vex];
            vis[next_vex] = 1;

        }


        // ���·��
        cout << "�Ƽ�·����ʼ�� \"" << v.name << "\": ";
        for (int i = 0; i < path_list.size() - 1; ++i) {
            cout << na[path_list[i]] << " -> ";
        }
        cout << na[path_list.back()] << endl;

        //���·������
        cout << "�ܾ���: " << totalLength << endl;

    }

    // ����Ƽ�·��2
    void print_recommended_path_2(Vertex v, string na[]) {

        for (int i = 0; i < vexnum; i++) {
            visi[i] = false;
        }
        DFS(v.ID);
        for (int i = 0; i < vexnum; i++) {
            if (!visi[i]) {
                system("cls");
                cout << "                         �޷���ͨ����·��!!!!!" << endl;
                return;
            }
        }


        if (v.ID < 0 || v.ID >= vexnum) {
            cout << "Invalid vertex ID" << endl;
            return;
        }

        int vis[MVNum] = { 0 }; // ��Ƕ����Ƿ���ʹ�
        vis[v.ID] = 1;

        vector<int> path_list;
        path_list.push_back(v.ID);

        int totalLength = 0;

        while (!allVisited(vis)) {
            int prev[MVNum] = { 0 };
            int dist[MVNum] = { 0 };
            int next_vex = -1;

            // Ѱ����һ��δ���ʵĶ���
            for (int j = 0; j < vexnum; ++j) {
                if (!vis[j]) {
                    next_vex = j;
                    break;
                }
            }

            Dijkstra(path_list.back(), prev, dist);

            // �ҵ���һ������
            for (int j = 0; j < vexnum; ++j) {
                if (!vis[j]) {
                    next_vex = (next_vex == -1 || dist[j] < dist[next_vex]) ? j : next_vex;
                }
            }

            // ����·��
            vector<int> temp;
            for (int j = next_vex; j != path_list.back(); j = prev[j]) {
                temp.insert(temp.begin(), j);
            }
            path_list.insert(path_list.end(), temp.begin(), temp.end());

            totalLength += dist[next_vex];
            vis[next_vex] = 1;
        }



        // ��ӻص�����·��
        int prev[MVNum] = { 0 };
        int dist[MVNum] = { 0 };
        Dijkstra(path_list.back(), prev, dist);
        vector<int> temp;
        for (int j = v.ID; j != path_list.back(); j = prev[j]) {
            temp.insert(temp.begin(), j);
        }
        path_list.insert(path_list.end(), temp.begin(), temp.end());

        totalLength += dist[v.ID]; // �ص����ľ���
        vis[v.ID] = 1;


        // ���·��
        cout << "�Ƽ�·����ʼ����� \"" << v.name << "\": ";
        for (int i = 0; i < path_list.size() - 1; ++i) {
            cout << na[path_list[i]] << " -> ";
        }
        cout << na[path_list.back()] << endl;
        cout << "�ܾ���: " << totalLength << endl;


    }

};
