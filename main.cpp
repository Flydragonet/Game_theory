#include <bits/stdc++.h>
using namespace std;
int** v; int now_agent;
bool cmp(int x, int y)
{
	return v[now_agent][x] > v[now_agent][y];
}
int main()
{
	//--------------------initialization
	int n, m;
	cout << "Input the number of agents and goods: ";
	cin >> n >> m;
	int seed = 0;
	//cout << "Input the random function seed: ";
	//cin >> seed;
	srand(seed);
	v = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
	{
		if (i % 1000 == 0) cout << "Now initializing: " << i << "\n";
		v[i] = (int*)malloc(m * sizeof(int));
		for (int j = 0; j < m; j++)
			v[i][j] = rand() % 1000;
	}
	vector<vector<int> > A(n);
	//---------------------solve
	double timer = clock();
	int choose = 2; //0 for scan, 1 for queue, 2 for list
	if (choose == 0)
	{
		bool* chosen = (bool*)malloc(m * sizeof(bool)); memset(chosen, 0, m * sizeof(bool));
		int cnt = 0;
		for (int t = 0; t < m; t++)
		{
			if (t % 10000 == 0) cout << "Now processing: " << t << "\n";
			int maxindex, maxnumber = -1;
			for (int j = 0; j < m; j++)
				if (!chosen[j] && v[cnt][j] > maxnumber)
					maxindex = j, maxnumber = v[cnt][j];
			A[cnt].push_back(maxindex);
			chosen[maxindex] = true;
			cnt = cnt == n - 1 ? 0 : cnt + 1;
		}
	}
	else if (choose == 1)
	{
		bool* chosen = (bool*)malloc(m * sizeof(bool)); memset(chosen, 0, m * sizeof(bool));
		int cnt = 0;
		vector<priority_queue<pair<int, int> > > q(n);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				q[i].push(make_pair(v[i][j], j));
		for (int t = 0; t < m; t++)
		{
			if (t % 10000 == 0) cout << "Now processing: " << t << "\n";
			pair<int, int> p = q[cnt].top(); q[cnt].pop();
			while (chosen[p.second])
			{
				p = q[cnt].top();
				q[cnt].pop();
			}
			A[cnt].push_back(p.second);
			chosen[p.second] = true;
			cnt = cnt == n - 1 ? 0 : cnt + 1;
		}
	}
	else if (choose == 2)
	{
		bool* chosen = (bool*)malloc(m * sizeof(bool)); memset(chosen, 0, m * sizeof(bool));
		int cnt = 0;
		int** li = (int**)malloc(n * sizeof(int*));
		for (int i = 0; i < n; i++)
		{
			if (i % 1000 == 0) cout << "Now ordering: " << i << "\n";
			vector<vector<int> > b(1000);
			li[i] = (int*)malloc(m * sizeof(int));
			for (int j = 0; j < m; j++)
			{
				b[v[i][j]].push_back(j);
			}
			int g_cnt = 0;
			for (int k = 999; k >= 0; k--)
				for (auto j : b[k])
					li[i][g_cnt++] = j;
		}
		int* p = (int*)malloc(n * sizeof(int)); memset(p, 0, n * sizeof(int));
		for (int t = 0; t < m; t++)
		{
			if (t % 10000 == 0) cout << "Now processing: " << t << "\n";
			while (chosen[li[cnt][p[cnt]]]) p[cnt]++;
			A[cnt].push_back(li[cnt][p[cnt]]);
			chosen[li[cnt][p[cnt]]] = true;
			cnt = cnt == n - 1 ? 0 : cnt + 1;
		}
	}
	timer = clock() - timer;
	//----------------------display
	if (choose == 0) cout << "Algorithm: SCAN\n";
	else if (choose == 1) cout << "Algorithm: QUEUE\n";
	else cout << "Algorithm: LIST\n";
	cout << "every one's pay off:\n";
	int* sum = (int*)malloc(n * sizeof(int)); memset(sum, 0, n * sizeof(int));
	for (int i = 0; i < n; i++)
		for (auto j : A[i])
			sum[i] += v[i][j];
	for (int i = 0; i < n; i++)
		cout << i << " = " << sum[i] << ", ";
	cout << "\n\n";
	cout << "runtime: " << timer / CLOCKS_PER_SEC << "\n";
	return 0;
}
