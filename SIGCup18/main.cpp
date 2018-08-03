#include <bits/stdc++.h>

using ll = long long;
using db = double;
using vi = std::vector<int>;
using pii = std::pair<int, int>;

#define AddEdge(x, y, z) num[tail[x] = next[tail[x]] = ++ etot] = y; val[etot] = z;

const int N = 11111111;

std::string vertex_id[N], edge_id[N];
namespace oo {
	int num[N], tail[N], next[N], etot;
	std::string val[N];
	int belong[N];

	int dfn[N], low[N]; int cnt = 0;
	bool cut[N]; std::stack<int> sta; std::vector<std::vector<int>> bcc;

	int tarjan(int x, int pe) {
		dfn[x] = low[x] = ++cnt; int ecnt = 0;
		for (int e = next[x]; e; e = next[e]) if (e != pe) {
			int i = num[e];
			if (dfn[i] < dfn[x]) sta.push(e);
			if (!dfn[i]) {
				low[x] = std::min(low[x], tarjan(i, e ^ 1));
				++ecnt;
				if (low[i] >= dfn[x]) {
					bcc.push_back({});
					do {
						bcc.back().push_back(sta.top()); sta.pop();
					} while (bcc.back().back() != e);
					if (pe) cut[x] = 1;
				}
			}
			else
				low[x] = std::min(low[x], dfn[i]);
		}
		if (!pe && ecnt > 1) cut[x] = 1;
		return low[x];
	}
}

using namespace oo;

bool starts[N], ends[N];
int edgedone[N];

namespace nn {
	int num[N], tail[N], next[N], val[N], etot, fa[N];
	std::vector<int> roots;
	bool isstart[N], isend[N];
	int startnum[N], endnum[N], treestart[N], treeend[N];
	int n;
	std::vector<std::vector<std::string>> strin;

	void dfs(int k) {
		for (int i = next[k]; i; i = next[i])
			if (!fa[num[i]]) {
				fa[num[i]] = k;
				dfs(num[i]);
				startnum[k] += startnum[num[i]];
				endnum[k] += endnum[num[i]];
			}
		startnum[k] += isstart[k];
		endnum[k] += isend[k];
	}

	std::vector<std::string> solve(int oldn, std::vector<std::vector<int>> &bcc, int *belong) {
		n = 0;
		for (int i = 1; i <= oldn; i++)
			n += cut[i];
		int cutn = n;
		n += bcc.size();
		for (int i = 1; i <= n; i++)
			tail[i] = i;
		etot = n + 1;
		strin.push_back({});
		for (int i = n; i--; strin.push_back({}));
		if ((etot & 1) == 0) etot++;
		int nowcut = 0;
		for (int i = 1; i <= oldn; i++)
			if (cut[i]) {
				nowcut++;
				strin[nowcut].push_back(vertex_id[i]);
				if (starts[i]) isstart[nowcut] = 1;
				if (ends[i]) isend[nowcut] = 1;
				for (int j = oo::next[i]; j; j = oo::next[j])
					if (edgedone[cutn + belong[j] + 1] != nowcut) {
						edgedone[cutn + belong[j] + 1] = nowcut;
						AddEdge(nowcut, cutn + belong[j] + 1, 0);
						AddEdge(cutn + belong[j] + 1, nowcut, 0);
					}
			}
		/*
		for (int i = 1; i <= n; i++) {
			std::cout << i << std::endl;
			for (int j = next[i]; j; j = next[j])
				std::cout << num[j] << ' ';
			std::cout << std::endl;
		}
		*/
		memset(edgedone, 255, sizeof edgedone);
		for (int i = 0; i < bcc.size(); i++) {
			auto &bb = bcc[i];
			int set = i + cutn + 1;
			for (int j : bb) {
				//std::cout << set << ' ' << oo::val[j] << ' ' << oo::num[j] << oo::num[j ^ 1] << std::endl;
				if (edgedone[j] != i) {
					strin[set].push_back(oo::val[j]);
					edgedone[j] = i;
				}
				if (edgedone[oo::num[j]] != i && !(cut[oo::num[j]] && bb.size() < 2)) {
					strin[set].push_back(vertex_id[oo::num[j]]);
					if (!cut[oo::num[j]]) {
						if (starts[oo::num[j]]) isstart[set] = 1;
						if (ends[oo::num[j]]) isend[set] = 1;
					}
					edgedone[oo::num[j]] = i;
				}
				if (edgedone[oo::num[j ^ 1]] != i && !(cut[oo::num[j ^ 1]] && bb.size() < 2)) {
					strin[set].push_back(vertex_id[oo::num[j ^ 1]]);
					if (!cut[oo::num[j ^ 1]]) {
						if (starts[oo::num[j ^ 1]]) isstart[set] = 1;
						if (ends[oo::num[j ^ 1]]) isend[set] = 1;
					}
					edgedone[oo::num[j ^ 1]] = i;
				}
			}
		}
		for (int i = 1; i <= n; i ++ )
			if (!fa[i]) {
				fa[i] = -1;
				roots.push_back(i);
				dfs(i);
			}
		std::queue<int> qq;
		for (auto i : roots)
			qq.push(i);
		for (; qq.size(); ) {
			auto t = qq.front();
			qq.pop();
			if (fa[t] == -1) {
				treestart[t] = startnum[t];
				treeend[t] = endnum[t];
			}
			else {
				treestart[t] = treestart[fa[t]];
				treeend[t] = treeend[fa[t]];
			}
			for (int i = next[t]; i; i = next[i])
				if (fa[num[i]] == t)
					qq.push(num[i]);
		}
		std::vector<std::string> ans;
		for (int i = 1; i <= n; i++) {
			if (isstart[i] && isend[i]) {
				for (auto &j : strin[i])
					ans.push_back(j);
			}
			else {
				int st = 0, ed = 0, sted = 0;
				if (isstart[i]) st++;
				if (isend[i]) ed++;
				int remainst = treestart[i] - st, remained = treeend[i] - ed;
				for (int j = next[i]; j; j = next[j])
					if (fa[num[j]] == i) {
						if (startnum[num[j]] && endnum[num[j]]) sted++;
						else if (startnum[num[j]]) st++;
						else if (endnum[num[j]]) ed++;
						remainst -= startnum[num[j]];
						remained -= endnum[num[j]];
					}
				if (remainst && remained) sted++;
				else if (remainst) st++;
				else if (remained) ed++;
				if ((st && (ed || sted)) || (ed && (st || sted)) || (sted > 1)) {
					for (auto j : strin[i])
						ans.push_back(j);
				}
			}
		}
		return ans;
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	std::ios::sync_with_stdio(0);
	int n, m;
	std::cin >> n >> m;
	etot = n + 1;
	if ((etot & 1) == 0) etot++;
	for (int i = 1; i <= n; i++)
		tail[i] = i;
	for (int i = 1; i <= n; i++)
		std::cin >> vertex_id[i];
	for (int i = 1; i <= m; i++) {
		int u, v;
		std::string s;
		std::cin >> u >> v >> s;
		AddEdge(u, v, s);
		AddEdge(v, u, s);
	}
	int sn, en, check = -1;
	std::cin >> sn;
	if (sn == 1) check = -2;
	for (; sn--; ) {
		int t;
		std::cin >> t;
		starts[t] = 1;
		if (check == -2) check = t;
	}
	std::cin >> en;
	if (en != 1) check = -1;
	for (; en--; ) {
		int t;
		std::cin >> t;
		ends[t] = 1;
		if (check != t) check = -1;
	}

	//if srart and end is one same point, return it
	if (check > 0) {
		std::cout << vertex_id[check];

		return 0;
	}

	for (int i = 1; i <= n; i++)
		if (!dfn[i]) tarjan(i, 0);

	int cuttot = 0;
	for (int i = 1; i <= n; i++)
		cuttot += cut[i];
	for (int i = 0; i < bcc.size(); i++)
		for (int j : bcc[i])
			belong[j] = belong[j ^ 1] = i;
	auto ans = nn::solve(n, bcc, belong);
	std::set<std::string> ansset;
	//for (auto &i : ans)
	//	ansset.insert(i);
	freopen("result.txt", "w", stdout);
	for (auto &i : ans)
		std::cout << i << '\n';

	return 0;
}