#include"head.h"
#include"Graph.h"

//0.�û������˵�
void screen() {
	int matching;
	while (1)
	{
		cout << "                         ��ӭ����ʦ������������" << endl;
		cout << "                         ���Ǽ�ά˹�����ĸ���У԰�滮����ϵͳ" << endl;
		cout << "                         �뿪ʼ��������" << endl;
		cout << "                         1. �鿴У԰��ͼ" << endl;
		cout << "                         2. ����У�ڵص�" << endl;
		cout << "                         3. ����У�ڵ�·" << endl;
		cout << "                         4. ���ص�·���" << endl;
		cout << "                         5. ��̵��߲���" << endl;
		cout << "                         6. ��̷�̸·��" << endl;
		cout << "                         7. �������·��" << endl;
		cout << "                         8. �������Ŀ��" << endl;

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
			cout << "                         ��ѡ�����Ĳ�����1.��ӵص� 2.ɾ���ص�" << endl;
			int t;
			cin >> t;
			if (t == 1)
				Add_Location();
			else if (t == 2)
				Delete_Location();
			else
			{
				cout << "                         ��������!!!" << endl;
			}
			break;
		}
		case 3:
		{
			Precent_Path();
			cout << endl;
			cout << "                         ��ѡ�����Ĳ�����1.��ӵ�· 2.ɾ����·" << endl;
			int t;
			cin >> t;
			if (t == 1)
				Add_Path();
			else if (t == 2)
				Delete_Path();
			else
			{
				cout << "                         ��������!!!" << endl;
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
			cout << "                         �ܸ���Ϊ������ף����������" << endl;
			return;
		}
		default:
		{
			cout << "                         ���Ĳ���Ŀǰ�����ҵ�������Χ��" << endl;
			break;
		}
		}
		system("pause");
		system("cls");

	}

}
void CreateMap(int choose) {

	// 1. ��ȡ�ص�����
		// ��ʼ������
	int sum = 0;
	string name[MVNum];
	int x[MVNum], y[MVNum];
	string info[MVNum];
	// ��txt�ļ��ж�ȡ����
	ifstream inputFile("Location_Information.txt");
	string line;
	// һ��һ�ж�ȡ�����������
	while (getline(inputFile, line)) {
		istringstream iss(line);
		iss >> name[sum] >> x[sum] >> y[sum] >> info[sum];
		sum++;
	}

	inputFile.close();

	// 2.��ȡ·����Ϣ
		// �����ά���飬����ʼ��
	int pa[MVNum][MVNum];
	for (int i = 0; i < sum; ++i) {
		for (int j = 0; j < sum; ++j) {
			pa[i][j] = 0;
		}
	}

	// ��ȡtxt�ļ�
	ifstream pathFile("Path_Information.txt");
	string pathLine;

	// Ƕ��ѭ��������ά����
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

	// 3.��Ϊ��ȫͼ
	for (int i = 0; i < sum; ++i) {
		for (int j = 0; j < sum; ++j) {
			if (!pa[i][j])
				pa[i][j] = INF;
		}
	}

	// 4.����ѡ��
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

//1. �鿴У԰��ͼ
void PrintMap(string name[MVNum], int x[MVNum], int y[MVNum], int sum) {

	//1.��ͼ����
		// ������ά��������ʾ��ͼ
	string loc[MVNum][MVNum]; 
	// ��ʼ����ͼ����
	for (int i = 0; i < MVNum; ++i) {
		for (int j = 0; j < MVNum; ++j) {
			loc[i][j] = "    "; // �ĸ��ո��ʾһ��λ��
		}
	}
	int max_x = 0, max_y = 0; // ��¼���� y ����
	// ���ص���Ϣ��ӵ���ͼ������
	for (int i = 0; i < sum; ++i) {
		loc[x[i]][y[i]] = name[i]; // ���ص�������ӵ���Ӧ������λ��
		if (y[i] > max_y)
			max_y = y[i];
		if (x[i] > max_x)
			max_x = x[i];
	}
	cout << "                         " << "    ";

	// ��ӡ������עÿһ�е�����
	for (int i = 0; i <= max_y + 1; ++i) {
		if (i < 10)
			cout << " " << i << "  ";
		else
			cout << " " << i << " ";
	}
	cout << endl;

	// ��ӡ��ͼ
	for (int i = 0; i <= max_x + 1; ++i) {
		cout << "                         " << i << "   ";
		for (int j = 0; j <= max_y + 1; ++j) {
			cout << loc[i][j];
		}
		cout << endl;
	}


	// 2.�ص���Ϣ
	Precent_Location();


	// 3.·����Ϣ
	Precent_Path();

}
void Precent_Location()
{
	string filename = "Location_Information.txt"; // �ļ���

	// ���ļ�
	ifstream inFile(filename);

	// ����ļ��Ƿ�ɹ���
	if (!inFile) {
		cerr << "                         �޷����ļ�: " << filename << endl;
		return;
	}

	string line;

	cout << "                         ��ǰ�ص���Ϣ:" << endl;

	// ���ж�ȡ�ļ����ݲ����������̨
	while (getline(inFile, line)) {
		cout << "                         " << line << endl;
	}

	// �ر��ļ�
	inFile.close();
}
void Precent_Path()
{
	string filename = "Path_Information.txt"; // �ļ���

	// ���ļ�
	ifstream inFile(filename);

	// ����ļ��Ƿ�ɹ���
	if (!inFile) {
		cerr << "�޷����ļ�: " << filename << endl;
		return;
	}

	string line;

	cout << "                         ��ǰ��·��Ϣ:" << endl;

	// ���ж�ȡ�ļ����ݲ����������̨
	while (getline(inFile, line)) {
		cout << "                         " << line << endl;
	}

	// �ر��ļ�
	inFile.close();
}

//2. ����У�ڵص�
void Add_Location()
{
	cout << "                         �����ʽ���£�" << endl;
	cout << "                         ��У�ſ�Ϊ��ά����ϵ(0��0)��" << endl;
	cout << "                         �ص� ��άx���� ��άy���� �ص����" << endl;
	cout << "                         ���� 6 6 ����֮��" << endl;

	string name;
	cin >> name;

	int x, y;
	cin >> x >> y;

	string inf;
	cin >> inf;


	// �����ļ������
	ofstream outFile;

	// ʹ�� ios::app ��׷��ģʽ���ļ�
	outFile.open("Location_Information.txt", ios::app);

	if (outFile.is_open()) {

		outFile << name << " " << x << " " << y << " " << inf << endl;
		cout << "                         ��ӳɹ�������" << endl;
	}
	else {
		cerr << "                         ���ʧ�ܣ�����" << endl;
	}

	// �ر��ļ�
	outFile.close();

}
void Delete_Location() {
	//ɾ��˼·���ȶ�ȡ�������У�������ɾ���������������д��


	cout << "                         ������Ҫɾ�����к�(�����һ��Ϊ1)" << endl;
	int l;
	cin >> l;
	int sum = 0;
	string name[MVNum];
	int x[MVNum], y[MVNum];
	string info[MVNum];

	// ��txt�ļ��ж�ȡ����
	ifstream inputFile("Location_Information.txt");
	string line;

	// һ��һ�ж�ȡ�����������
	while (getline(inputFile, line)) {
		istringstream iss(line);
		iss >> name[sum] >> x[sum] >> y[sum] >> info[sum];
		sum++;
	}

	inputFile.close();

	// ��ָ���п�ʼɾ��
	for (int i = l - 1; i < sum - 1; i++) {
		name[i] = name[i + 1];
		x[i] = x[i + 1];
		y[i] = y[i + 1];
		info[i] = info[i + 1];
	}

	// ���ļ�����д��
	ofstream outputFile("Location_Information.txt", ios::trunc); // ���ļ����������

	// ����д������
	for (int i = 0; i < sum - 1; i++) {
		outputFile << name[i] << " " << x[i] << " " << y[i] << " " << info[i] << endl;
	}

	outputFile.close();

	cout << "                         ɾ���ɹ�������" << endl;
}

//3. ����У�ڵ�·
void Add_Path()
{

	string name1;
	cin >> name1;

	string name2;
	cin >> name2;

	// �����ļ������
	ofstream outFile;
	// ʹ�� ios::app ��׷��ģʽ���ļ�
	outFile.open("Path_Information.txt", ios::app);

	// ���ļ���д�� name1 �� name2
	if (outFile.is_open()) {
		outFile << name1 << " " << name2 << endl;
		cout << "                         ��ӳɹ�������" << endl;
	}
	else {
		cerr << "                         ���ܴ��ļ�! ! !" << endl;
	}

	// �ر��ļ�
	outFile.close();
}
void Delete_Path() {
	//ɾ��˼·���ȶ�ȡ�������У�������ɾ���������������д��

	cout << "������Ҫɾ�����к�(�����һ��Ϊ1)" << endl;
	int sum = 0, l;
	cin >> l;
	string pa1[MVNum], pa2[MVNum];


	// ��txt�ļ��ж�ȡ����
	ifstream inputFile("Path_Information.txt");
	string line;

	// һ��һ�ж�ȡ�����������
	while (getline(inputFile, line)) {
		istringstream iss(line);
		iss >> pa1[sum] >> pa2[sum];
		sum++;
	}

	inputFile.close();

	// ��ָ���п�ʼɾ��
	for (int i = l - 1; i < sum - 1; i++) {
		pa1[i] = pa1[i + 1];
		pa2[i] = pa2[i + 1];
	}

	// ���ļ�����д��
	ofstream outputFile("Path_Information.txt", ios::trunc); // ���ļ����������

	// ����д������
	for (int i = 0; i < sum - 1; i++) {
		outputFile << pa1[i] << " " << pa2[i] << endl;
	}

	outputFile.close();

	cout << "                         ɾ���ɹ�������" << endl;
}


//4. ���·��
void Path(int pa[MVNum][MVNum], int sum, int choose, string name[]) {

	CreateMap(1);

	// ����ͼ
	Graph graph(sum);

	//��ӱ�
	for (int i = 0; i < sum; i++)
	{
		for (int j = i + 1; j < sum; j++)
		{
			graph.addEdge(i, j, pa[i][j]);
		}
	}

	// �����ʼ��
	vector<Vertex> vertices;                        //���嶥���������(��̬����)
	for (int i = 0; i < sum; i++)
	{
		vertices.push_back(Vertex(name[i], i));
	}


	if (choose == 4) {
		int prev[MVNum];
		int dist[MVNum];

		//��������
		cout << "                         ����������λ�ã�" << endl;
		cout << "                         ";
		for (int i = 0; i < sum; i++)
		{
			cout << i << "." << name[i] << " ";
		}
		int k;
		cin >> k;
		if (k >= sum || k < 0)
		{
			cout << "                         �������󣡣�����" << endl;
			return;
		}

		graph.Dijkstra(k, prev, dist);
		graph.printShortestPathsFromSource(k, prev, dist, name); // ����name�Ƕ�����������
	}
	else if (choose == 5)
		graph.mintree(name);
	else
	{
		// ��ӡ�Ƽ�·��
		for (int i = 0; i < vertices.size(); ++i) {		//����i��ʼ
			Vertex v = vertices[i];
			if (choose == 6)
				graph.print_recommended_path_1(v, name);
			else
				graph.print_recommended_path_2(v, name);
		}
	}
	return;
}