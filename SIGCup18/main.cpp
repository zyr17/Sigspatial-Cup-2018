#include <bits/stdc++.h>

#define InputJSON "test.json"
#define StartPointFile "teststart.txt"
#define OutputFile "result.txt"

namespace parse {
	struct Edge {
		int x, y;
		std::string id;
		bool isstart, isend;
	};
	std::vector<int> S;
	std::vector<int> T;
	std::vector<std::string> points;
	std::vector<Edge> edges;

	struct Tree {
		int next[16], pointid, edgevecnum;
	};
	std::vector<Tree> tree;
	int A2I[256];
	int stringpos;
	std::string str;
	const int FindControllers = -100, EndOfFile = -1;
	const char controllers[] = "controllers";
	std::vector<std::vector<int>> id2e;

	int gettoken(const char *input) {
		for (;;) {
			for (; stringpos < str.size() && str[stringpos] != '\"'; stringpos++);
			if (stringpos == str.size()) return EndOfFile;
			int tokenend = ++stringpos;
			for (; tokenend < str.size() && str[tokenend] != '\"'; tokenend++);
			if (tokenend == str.size()) return EndOfFile;
			bool flag = 1;
			for (int i = 0; flag && i + stringpos < tokenend; i++)
				if (input[i] != str[stringpos + i]) flag = 0;
			flag = flag && (input[tokenend - stringpos] == 0);
			if (flag) {
				stringpos = tokenend + 1;
				return 0;
			}
			flag = 1;
			for (int i = 0; flag && i + stringpos < tokenend; i++)
				if (controllers[i] != str[stringpos + i]) flag = 0;
			flag = flag && (controllers[tokenend - stringpos] == 0);
			if (flag) {
				stringpos = tokenend + 1;
				return FindControllers;
			}
		}
	}

	void getnext(std::string &out) {
		out = "";
		for (; stringpos < str.size() && str[stringpos] != '\"'; stringpos++);
		if (stringpos == str.size()) return;
		int tokenend = ++stringpos;
		for (; tokenend < str.size() && str[tokenend] != '\"'; tokenend++);
		if (tokenend == str.size()) return;
		out = str.substr(stringpos, tokenend - stringpos);
		stringpos = tokenend + 1;
	}

	Tree &getleaf(std::string &id) {
		int now = 0;
		for (auto i : id) {
			int childnum = A2I[i];
			if (childnum == -1) continue;
			if (tree[now].next[childnum] == 0) {
				tree[now].next[childnum] = tree.size();
				tree.push_back(Tree());
			}
			now = tree[now].next[childnum];
		}
		return tree[now];
	}

	void addedge(std::string &id, int edgeid) {
		auto &leaf = getleaf(id);
		if (!leaf.edgevecnum) {
			leaf.edgevecnum = id2e.size();
			id2e.push_back(std::vector<int>());
		}
		id2e[leaf.edgevecnum].push_back(edgeid);
	}

	int addpoint(std::string &id) {
		auto &leaf = getleaf(id);
		if (!leaf.pointid) {
			points.push_back(id);
			leaf.pointid = points.size();
		}
		return leaf.pointid;
	}

	void setSE(std::string &id, bool isstart) {
		auto &leaf = getleaf(id);
		if (leaf.pointid) {
			if (isstart) S.push_back(leaf.pointid);
			else T.push_back(leaf.pointid);
		}
		if (leaf.edgevecnum)
			for (auto i : id2e[leaf.edgevecnum]) {
				if (isstart) edges[i].isstart = 1;
				else edges[i].isend = 1;
			}
	}

	void parse() {
		std::ios::sync_with_stdio(0);
		freopen(InputJSON, "r", stdin);
		std::string temp;
		std::getline(std::cin, str);
		while (std::getline(std::cin, temp))
			str += temp;
		id2e.push_back(std::vector<int>());
		tree.push_back(Tree());
		memset(A2I, 255, sizeof A2I);
		A2I['0'] = 0;
		A2I['1'] = 1;
		A2I['2'] = 2;
		A2I['3'] = 3;
		A2I['4'] = 4;
		A2I['5'] = 5;
		A2I['6'] = 6;
		A2I['7'] = 7;
		A2I['8'] = 8;
		A2I['9'] = 9;
		A2I['A'] = 10;
		A2I['B'] = 11;
		A2I['C'] = 12;
		A2I['D'] = 13;
		A2I['E'] = 14;
		A2I['F'] = 15;
		gettoken("rows");
		for (;;) {
			if (gettoken("viaGlobalId") == FindControllers) break;
			edges.push_back(Edge());
			getnext(edges.back().id);
			addedge(edges.back().id, edges.size() - 1);
			gettoken("fromGlobalId");
			getnext(temp);
			edges.back().x = addpoint(temp);
			gettoken("toGlobalId");
			getnext(temp);
			edges.back().y = addpoint(temp);
		}
		for (;;) {
			if (gettoken("globalId") == EndOfFile) break;
			getnext(temp);
			setSE(temp, 1);
		}
		freopen(StartPointFile, "r", stdin);
		std::cin.clear();
		while (std::getline(std::cin, temp))
			setSE(temp, 0);
	}
}

#define AddEdge(x, y, z) num[tail[x] = next[tail[x]] = ++ etot] = y; val[etot] = z;

std::vector<int> vertex_id;
std::vector<std::string> strings;
namespace oo {
	std::vector<int> num, tail, next, val, belong;
	int etot;

	std::vector<int> dfn, low;
	int cnt = 0;
	std::vector<bool> cut;
	std::stack<int> sta;
	std::vector<std::vector<int>> bcc;

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

std::vector<bool> starts, ends;
std::vector<int> edgedone;

namespace nn {
	std::vector<int> num, tail, next, val, fa;
	int etot;
	std::vector<int> roots;
	std::vector<bool> isstart, isend;
	std::vector<int> startnum, endnum, treestart, treeend;
	int n;
	std::vector<std::vector<int>> strin;

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

	std::vector<int> solve(int oldn, std::vector<std::vector<int>> &bcc, std::vector<int> &belong) {
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
		memset(&(edgedone[0]), 255, sizeof(int) * edgedone.size());
		for (int i = 0; i < bcc.size(); i++) {
			auto &bb = bcc[i];
			int set = i + cutn + 1;
			for (int j : bb) {
				if (edgedone[j] != i) {
					strin[set].push_back(oo::val[j]);
					edgedone[j] = i;
					if (starts[j]) isstart[set] = 1;
					if (ends[j]) isend[set] = 1;
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
		std::vector<int> ans;
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

void nnresizevector(int n, int m) {
	int N = n + m * 2 + 1111;
	nn::num.resize(N);
	nn::tail.resize(N);
	nn::next.resize(N);
	nn::val.resize(N);
	nn::fa.resize(N);
	nn::isstart.resize(N);
	nn::isend.resize(N);
	nn::num.resize(N);
	nn::startnum.resize(N);
	nn::endnum.resize(N);
	nn::treestart.resize(N);
	nn::treeend.resize(N);
}
void resizevector(int n, int m) {
	int N = n + m * 2 + 1111;
	vertex_id.resize(N);
	num.resize(N);
	tail.resize(n + 1111);
	next.resize(N);
	val.resize(N);
	belong.resize(N);
	dfn.resize(n + 1111);
	low.resize(n + 1111);
	cut.resize(n + 1111);
	starts.resize(N);
	ends.resize(N);
	edgedone.resize(N);
	nnresizevector(n + m, m * 2);
}

void input(int &n, int &m, int &check) {
	parse::parse();

	n = parse::points.size();
	m = parse::edges.size();

	resizevector(n, m);
	etot = n + 1;
	if ((etot & 1) == 0) etot++;
	for (int i = 1; i <= n; i++)
		tail[i] = i;
	for (int i = 1; i <= n; i++) {
		vertex_id[i] = strings.size();

		strings.push_back(parse::points[i - 1]);

	}
	int sttot = 0, edtot = 0, lasts, laste;
	for (int i = 1; i <= m; i++) {
		int u, v, s = strings.size();

		u = parse::edges[i - 1].x;
		v = parse::edges[i - 1].y;
		strings.push_back(parse::edges[i - 1].id);

		AddEdge(u, v, s);
		AddEdge(v, u, s);

		starts[etot] = starts[etot - 1] = parse::edges[i - 1].isstart;
		ends[etot] = ends[etot - 1] = parse::edges[i - 1].isend;
	}
	int sn, en;

	sn = parse::S.size();

	for (auto t : parse::S) {

		starts[t] = 1;
		lasts = t;
		sttot++;
	}

	en = parse::T.size();

	for (auto t : parse::T) {

		ends[t] = 1;
		laste = t;
		edtot++;
	}
	if (1 == edtot && sttot == 1 && lasts == laste)
		check = lasts;
}

int main() {
	//freopen("input.txt", "r", stdin);
	freopen(OutputFile, "w", stdout);
	std::ios::sync_with_stdio(0);
	int n, m, check = -1;
	input(n, m, check);

	//if srart and end is one same feature, return it
	if (check > 0) {
		std::cout << strings[vertex_id[check]];

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
	//sort and remove duplicate
	//for (auto &i : ans)
	//	ansset.insert(strings[i]);
	for (auto &i : ans)
		std::cout << strings[i] << '\n';

	return 0;
}