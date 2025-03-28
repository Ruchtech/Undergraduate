#pragma once
#include<iostream>
#include<string>
#include<fstream>	//写入文档
#include <sstream>	//读取文档
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

#define INF 9999    //定义无路径
#define MVNum 1000  //定义最大地点数

//0.用户操作菜单
void screen();
void CreateMap(int choose);

//1. 查看校园蓝图
void PrintMap(string name[MVNum], int x[MVNum], int y[MVNum], int sum);
void Precent_Location();
void Precent_Path();

//2. 构建校内地点
void Add_Location();
void Delete_Location();

//3. 构建校内道路
void Add_Path();
void Delete_Path();

//4. 最短路线问题
void Path(int pa[MVNum][MVNum], int sum, int choose, string name[]);