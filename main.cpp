#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
using namespace std;
struct Change
{
	int u=0;
	int d=0;
	int delt=1000000;
};
clock_t start, endc;
long long Maxiter = 100000000;
int** m = NULL;
int** act = NULL;
int** tabu = NULL;
int best_f = 0;
int read_data(int** & matrix) {
	string file_path = "C:\\Users\\cardal\\Desktop\\研究生study\\data\\DSJC125.5.col";
	string s;
	ifstream infile;
	infile.open(file_path);
	int n;
	while (getline(infile, s)) {
		if (s.at(0) == 'p') {
			s = s.substr(7);
			n = stoi(s.substr(0, s.find_first_of(' ')));
			matrix = new int*[n];
			for (int i = 0; i < n; i++) {
				matrix[i] = new int[n];
				for (int j = 0; j < n; j++) {
					matrix[i][j] = 0;
				}
			}
		}
		if (s.at(0) == 'e') {
			s = s.substr(2);
			int e1= stoi(s.substr(0, s.find_first_of(' ')));
			int e2 = stoi(s.substr(s.find_first_of(' ')+1,s.length()- s.find_first_of(' ')-1));
			matrix[e1 - 1][e2 - 1] = 1;
			matrix[e2 - 1][e1 - 1] = 1;
		}
	}
	return n;
}
int init(int n,int* node,int** &act,int k,int** m,int** &tabu) {
	act = new int*[n];
	tabu = new int*[n];
	int f = 0;
	for (int i = 0; i < n; i++) {
		act[i] = new int[k];
		tabu[i] = new int[k];
		for (int j = 0; j < k; j++) {
			act[i][j] = 0;
			tabu[i][j] = 0;
		}
		node[i] = rand() % k;
	}
	for (int i = 0; i < n; i++) {
		for (int j = i+1; j < n; j++) {
			if (m[i][j] == 1) {
				act[i][node[j]]++;
				act[j][node[i]]++;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		f += act[i][node[i]];
	}
	return f;
}
void find_move(Change &c,int* node,int n,int k,long long iter,int f) {
	Change c_t;
	int com_t = 0;
	Change c_n;
	int com_n = 0;
	for (int i = 0; i < n; i++) {
		if (act[i][node[i]] != 0) {
			for (int j = 0; j < k; j++) {
				if (j != node[i]) {
					int del = (act[i][j]-act[i][node[i]])*2;
					if (iter < tabu[i][j]) {
						if (c_t.delt > del) {
							c_t.u = i;
							c_t.d = j;
							c_t.delt = del;
							com_t = 0;
						}
						else if (c_t.delt == del) {
							if (rand()%(com_t+2)==0) {
								c_t.u = i;
								c_t.d = j;
							}
							com_t++;
						}
					}
					else {
						if (c_n.delt > del) {
							c_n.u = i;
							c_n.d = j;
							c_n.delt = del;
							com_n = 0;
						}
						else if (c_n.delt == del) {
							if (rand() % (com_n + 2) == 0) {
								c_n.u = i;
								c_n.d = j;
							}
							com_n++;
						}
					}
				}
			}
		}
	}
	if (c_t.delt < c_n.delt&&f + c_t.delt < best_f) {
		c = c_t;
	}
	else {
		c = c_n;
	}
}
void make_move(Change c, int* node, int n, int k, long long iter,int &f) {
	tabu[c.u][node[c.u]] = iter + f/2 + rand() % 10+1;
	for (int i = 0; i < n; i++) {
		if (m[c.u][i] == 1) {
			act[i][node[c.u]]--;
			act[i][c.d]++;
		}
	}
	node[c.u] = c.d;
	f += c.delt;
	if (f < best_f) {
		best_f = f;
	}
}
int main() {
	srand(time(0));
	int n=0,k=0,f= 0;
	long long iter = 0;
	Change c;
	n=read_data(m);
	int* node = new int[n];
	cout << "请输入此图尝试着色的颜色数：" << endl;
	cin >> k;
	start = clock();
	f=init(n, node, act, k, m,tabu);
	best_f = f;
	while (iter < Maxiter) {
		find_move(c,node,n,k,iter,f);
		make_move(c,node,n,k,iter,f);
		if (iter % 1000000 == 0) {
			cout << "目前迭代次数为："<< iter <<",继续迭代着色中..."<< endl;
		}
		if (f == 0) {
			break;
		}
		iter++;
	}
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (m[i][j] == 1) {
				if (node[i] == node[j]) {
					cout << "存在不合法着色" << endl;
				}
			}
		}
	}
	if (iter < Maxiter) {
		endc = clock();
		cout << "着色成功，迭代次数为："<<iter << endl;
		cout << "着色所用时间：" << (double)(endc - start) / CLOCKS_PER_SEC;
	}
	else {
		cout << "着色失败" << endl;
	}
	system("pause");
}