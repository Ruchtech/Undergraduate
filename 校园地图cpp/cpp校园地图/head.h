#pragma once
#include<iostream>
#include<string>
#include<fstream>	//д���ĵ�
#include <sstream>	//��ȡ�ĵ�
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

#define INF 9999    //������·��
#define MVNum 1000  //�������ص���

//0.�û������˵�
void screen();
void CreateMap(int choose);

//1. �鿴У԰��ͼ
void PrintMap(string name[MVNum], int x[MVNum], int y[MVNum], int sum);
void Precent_Location();
void Precent_Path();

//2. ����У�ڵص�
void Add_Location();
void Delete_Location();

//3. ����У�ڵ�·
void Add_Path();
void Delete_Path();

//4. ���·������
void Path(int pa[MVNum][MVNum], int sum, int choose, string name[]);