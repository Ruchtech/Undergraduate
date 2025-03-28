#include"head.h"
#include"Graph.h"

//0.用户操作菜单
void screen() {
	int matching;
	while (1)
	{
		cout << "                         欢迎工程师回来！！！！" << endl;
		cout << "                         我是贾维斯，您的个人校园规划辅助系统" << endl;
		cout << "                         请开始！！！！" << endl;
		cout << "                         1. 查看校园蓝图" << endl;
		cout << "                         2. 构建校内地点" << endl;
		cout << "                         3. 构建校内道路" << endl;
		cout << "                         4. 各地道路情况" << endl;
		cout << "                         5. 最短电线布局" << endl;
		cout << "                         6. 最短访谈路线" << endl;
		cout << "                         7. 最短外卖路线" << endl;
		cout << "                         8. 已完成项目！" << endl;

		cin >> matching;
		cout << matching;

		switch (matching) {
		case 1:
		{
			CreateMap(1);
			break;
		}
		case 2:
		{
			Precent_Location();
			cout << endl;
			cout << "                         请选择您的操作：1.添加地点 2.删除地点" << endl;
			int t;
			cin >> t;
			if (t == 1)
				Add_Location();
			else if (t == 2)
				Delete_Location();
			else
			{
				cout << "                         输入有误!!!" << endl;
			}
			break;
		}
		case 3:
		{
			Precent_Path();
			cout << endl;
			cout << "                         请选择您的操作：1.添加道路 2.删除道路" << endl;
			int t;
			cin >> t;
			if (t == 1)
				Add_Path();
			else if (t == 2)
				Delete_Path();
			else
			{
				cout << "                         输入有误!!!" << endl;
			}
			break;
		}
		case 4:
		{
			CreateMap(4);
			break;
		}
		case 5:
		{
			CreateMap(5);
			break;
		}
		case 6:
		{
			CreateMap(6);
			break;
		}
		case 7:
		{
			CreateMap(7);
			break;
		}
		case 8:
		{
			system("cls");
			cout << "                         很高兴为您服务，祝您生活如意" << endl;
			return;
		}
		default:
		{
			cout << "                         您的操作目前不在我的能力范围内" << endl;
			break;
		}
		}
		system("pause");
		system("cls");

	}

}
void CreateMap(int choose) {

	// 1. 读取地点数据
		// 初始化定义
	int sum = 0;
	string name[MVNum];
	int x[MVNum], y[MVNum];
	string info[MVNum];
	// 从txt文件中读取数据
	ifstream inputFile("Location_Information.txt");
	string line;
	// 一行一行读取，存入变量中
	while (getline(inputFile, line)) {
		istringstream iss(line);
		iss >> name[sum] >> x[sum] >> y[sum] >> info[sum];
		sum++;
	}

	inputFile.close();

	// 2.读取路径信息
		// 定义二维数组，并初始化
	int pa[MVNum][MVNum];
	for (int i = 0; i < sum; ++i) {
		for (int j = 0; j < sum; ++j) {
			pa[i][j] = 0;
		}
	}

	// 读取txt文件
	ifstream pathFile("Path_Information.txt");
	string pathLine;

	// 嵌套循环遍历二维数组
	while (getline(pathFile, pathLine)) {
		istringstream iss(pathLine);
		string name1, name2;

		iss >> name1 >> name2;

		int hang = -1, lie = -1;
		for (int k = 0; k < sum; k++) {
			if (name[k] == name1)
				hang = k;
			if (name[k] == name2)
				lie = k;
		}
		if (hang != -1 && lie != -1) {
			pa[hang][lie] = pa[lie][hang] = round(sqrt((x[hang] - x[lie]) * (x[hang] - x[lie]) + (y[hang] - y[lie]) * (y[hang] - y[lie])));
		}
	}

	pathFile.close();

	// 3.化为完全图
	for (int i = 0; i < sum; ++i) {
		for (int j = 0; j < sum; ++j) {
			if (!pa[i][j])
				pa[i][j] = INF;
		}
	}

	// 4.操作选择
	if (choose == 1)
		PrintMap(name, x, y, sum);
	else if (choose == 4)
		Path(pa, sum, 4, name);
	else if (choose == 5)
		Path(pa, sum, 5, name);
	else if (choose == 6)
		Path(pa, sum, 6, name);
	else
		Path(pa, sum, 7, name);
}

//1. 查看校园蓝图
void PrintMap(string name[MVNum], int x[MVNum], int y[MVNum], int sum) {

	//1.地图构建
		// 创建二维数组来表示地图
	string loc[MVNum][MVNum]; 
	// 初始化地图数组
	for (int i = 0; i < MVNum; ++i) {
		for (int j = 0; j < MVNum; ++j) {
			loc[i][j] = "    "; // 四个空格表示一个位置
		}
	}
	int max_x = 0, max_y = 0; // 记录最大的 y 坐标
	// 将地点信息添加到地图数组中
	for (int i = 0; i < sum; ++i) {
		loc[x[i]][y[i]] = name[i]; // 将地点名称添加到对应的坐标位置
		if (y[i] > max_y)
			max_y = y[i];
		if (x[i] > max_x)
			max_x = x[i];
	}
	cout << "                         " << "    ";

	// 打印顶部标注每一列的坐标
	for (int i = 0; i <= max_y + 1; ++i) {
		if (i < 10)
			cout << " " << i << "  ";
		else
			cout << " " << i << " ";
	}
	cout << endl;

	// 打印地图
	for (int i = 0; i <= max_x + 1; ++i) {
		cout << "                         " << i << "   ";
		for (int j = 0; j <= max_y + 1; ++j) {
			cout << loc[i][j];
		}
		cout << endl;
	}


	// 2.地点信息
	Precent_Location();


	// 3.路径信息
	Precent_Path();

}
void Precent_Location()
{
	string filename = "Location_Information.txt"; // 文件名

	// 打开文件
	ifstream inFile(filename);

	// 检查文件是否成功打开
	if (!inFile) {
		cerr << "                         无法打开文件: " << filename << endl;
		return;
	}

	string line;

	cout << "                         当前地点信息:" << endl;

	// 逐行读取文件内容并输出到控制台
	while (getline(inFile, line)) {
		cout << "                         " << line << endl;
	}

	// 关闭文件
	inFile.close();
}
void Precent_Path()
{
	string filename = "Path_Information.txt"; // 文件名

	// 打开文件
	ifstream inFile(filename);

	// 检查文件是否成功打开
	if (!inFile) {
		cerr << "无法打开文件: " << filename << endl;
		return;
	}

	string line;

	cout << "                         当前道路信息:" << endl;

	// 逐行读取文件内容并输出到控制台
	while (getline(inFile, line)) {
		cout << "                         " << line << endl;
	}

	// 关闭文件
	inFile.close();
}

//2. 构建校内地点
void Add_Location()
{
	cout << "                         输入格式如下：" << endl;
	cout << "                         以校门口为二维坐标系(0，0)点" << endl;
	cout << "                         地点 二维x坐标 二维y坐标 地点介绍" << endl;
	cout << "                         北京 6 6 东方之都" << endl;

	string name;
	cin >> name;

	int x, y;
	cin >> x >> y;

	string inf;
	cin >> inf;


	// 定义文件输出流
	ofstream outFile;

	// 使用 ios::app 以追加模式打开文件
	outFile.open("Location_Information.txt", ios::app);

	if (outFile.is_open()) {

		outFile << name << " " << x << " " << y << " " << inf << endl;
		cout << "                         添加成功！！！" << endl;
	}
	else {
		cerr << "                         添加失败！！！" << endl;
	}

	// 关闭文件
	outFile.close();

}
void Delete_Location() {
	//删除思路：先读取至数组中，再数组删除操作，最后重新写入


	cout << "                         输入您要删除的行号(假设第一行为1)" << endl;
	int l;
	cin >> l;
	int sum = 0;
	string name[MVNum];
	int x[MVNum], y[MVNum];
	string info[MVNum];

	// 从txt文件中读取数据
	ifstream inputFile("Location_Information.txt");
	string line;

	// 一行一行读取，存入变量中
	while (getline(inputFile, line)) {
		istringstream iss(line);
		iss >> name[sum] >> x[sum] >> y[sum] >> info[sum];
		sum++;
	}

	inputFile.close();

	// 从指定行开始删除
	for (int i = l - 1; i < sum - 1; i++) {
		name[i] = name[i + 1];
		x[i] = x[i + 1];
		y[i] = y[i + 1];
		info[i] = info[i + 1];
	}

	// 打开文件进行写入
	ofstream outputFile("Location_Information.txt", ios::trunc); // 打开文件并清空内容

	// 重新写入数据
	for (int i = 0; i < sum - 1; i++) {
		outputFile << name[i] << " " << x[i] << " " << y[i] << " " << info[i] << endl;
	}

	outputFile.close();

	cout << "                         删除成功！！！" << endl;
}

//3. 构建校内道路
void Add_Path()
{

	string name1;
	cin >> name1;

	string name2;
	cin >> name2;

	// 定义文件输出流
	ofstream outFile;
	// 使用 ios::app 以追加模式打开文件
	outFile.open("Path_Information.txt", ios::app);

	// 在文件中写入 name1 和 name2
	if (outFile.is_open()) {
		outFile << name1 << " " << name2 << endl;
		cout << "                         添加成功！！！" << endl;
	}
	else {
		cerr << "                         不能打开文件! ! !" << endl;
	}

	// 关闭文件
	outFile.close();
}
void Delete_Path() {
	//删除思路：先读取至数组中，再数组删除操作，最后重新写入

	cout << "输入您要删除的行号(假设第一行为1)" << endl;
	int sum = 0, l;
	cin >> l;
	string pa1[MVNum], pa2[MVNum];


	// 从txt文件中读取数据
	ifstream inputFile("Path_Information.txt");
	string line;

	// 一行一行读取，存入变量中
	while (getline(inputFile, line)) {
		istringstream iss(line);
		iss >> pa1[sum] >> pa2[sum];
		sum++;
	}

	inputFile.close();

	// 从指定行开始删除
	for (int i = l - 1; i < sum - 1; i++) {
		pa1[i] = pa1[i + 1];
		pa2[i] = pa2[i + 1];
	}

	// 打开文件进行写入
	ofstream outputFile("Path_Information.txt", ios::trunc); // 打开文件并清空内容

	// 重新写入数据
	for (int i = 0; i < sum - 1; i++) {
		outputFile << pa1[i] << " " << pa2[i] << endl;
	}

	outputFile.close();

	cout << "                         删除成功！！！" << endl;
}


//4. 最短路线
void Path(int pa[MVNum][MVNum], int sum, int choose, string name[]) {

	CreateMap(1);

	// 创建图
	Graph graph(sum);

	//添加边
	for (int i = 0; i < sum; i++)
	{
		for (int j = i + 1; j < sum; j++)
		{
			graph.addEdge(i, j, pa[i][j]);
		}
	}

	// 顶点初始化
	vector<Vertex> vertices;                        //定义顶点类的向量(动态数组)
	for (int i = 0; i < sum; i++)
	{
		vertices.push_back(Vertex(name[i], i));
	}


	if (choose == 4) {
		int prev[MVNum];
		int dist[MVNum];

		//交互界面
		cout << "                         请输入您的位置：" << endl;
		cout << "                         ";
		for (int i = 0; i < sum; i++)
		{
			cout << i << "." << name[i] << " ";
		}
		int k;
		cin >> k;
		if (k >= sum || k < 0)
		{
			cout << "                         输入有误！！！！" << endl;
			return;
		}

		graph.Dijkstra(k, prev, dist);
		graph.printShortestPathsFromSource(k, prev, dist, name); // 假设name是顶点名称数组
	}
	else if (choose == 5)
		graph.mintree(name);
	else
	{
		// 打印推荐路径
		for (int i = 0; i < vertices.size(); ++i) {		//顶点i开始
			Vertex v = vertices[i];
			if (choose == 6)
				graph.print_recommended_path_1(v, name);
			else
				graph.print_recommended_path_2(v, name);
		}
	}
	return;
}