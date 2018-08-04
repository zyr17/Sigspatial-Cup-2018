#include <bits/stdc++.h>
#include "json.hpp"

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
	void parse() {
		std::ios::sync_with_stdio(0);
		//freopen("input", "w", stdout);

		freopen("EsriNapervilleElectricNetwork.json", "r", stdin);
		std::string s, t;
		while (std::getline(std::cin, s))
			t += s;

		nlohmann::json j = nlohmann::json::parse(t);
		std::map <std::string, int> disc;
		for (auto i : j["rows"]) {
			//cout << i << '\n';
			disc[i["fromGlobalId"]] = 0;
			disc[i["toGlobalId"]] = 0;
		}

		int n = 0;
		for (auto &i : disc)
			i.second = ++n;
		int m = j["rows"].size();
		
		//std::cout << n << ' ' << m << '\n';
		for (auto i : disc)
			points.push_back(i.first);
			//cout << i.first << '\n';

		for (auto i : j["rows"]) {
			edges.push_back(Edge());
			edges.back().x = disc[i["fromGlobalId"]];
			edges.back().y = disc[i["toGlobalId"]];
			edges.back().id = i["viaGlobalId"];
			/*
			std::cout << disc[i["fromGlobalId"]] << ' ';
			std::cout << disc[i["toGlobalId"]] << ' ';
			s = i["viaGlobalId"];
			std::cout << s << '\n';
			*/
		}
		
		for (auto i : j["controllers"]) {
			S.push_back(disc[i["globalId"]]);
		}
		freopen("startingpoints.txt", "r", stdin);
		std::cin.clear();
		while (std::getline(std::cin, s)) {
			//cout << s << '\n';
			T.push_back(disc[s]);
			//for(auto i : disc)
			//if(i.sec == 2159) {
			//cout << i.fir << '\n';
			//cout << (i.fir == s) << '\n';
			//cout << s.size() << ' ' << i.fir.size() << '\n';
			//}
		}
		
		/*
		std::cout << S.size() << '\n';
		for (int x : S) std::cout << x << '\n';
		std::cout << T.size() << '\n';
		for (int x : T) std::cout << x << '\n';
		*/
	}
	void parsetest() {
		freopen("input.txt", "r", stdin);
		std::ios::sync_with_stdio(0);
		int n, m;
		std::cin >> n >> m;
		for (; n--; ) {
			std::string s;
			std::cin >> s;
			points.push_back(s);
		}
		for (; m--; ) {
			edges.push_back(Edge());
			std::cin >> edges.back().x >> edges.back().y >> edges.back().id;
		}
		for (std::cin >> n; n--; ) {
			int t;
			std::cin >> t;
			S.push_back(t);
		}
		for (std::cin >> n; n--; ) {
			int t;
			std::cin >> t;
			T.push_back(t);
		}
		n = 0;
		for (std::cin >> n; n--; ) {
			int t;
			std::cin >> t;
			edges[t - 1].isstart = 1;
		}
		n = 0;
		for (std::cin >> n; n--; ) {
			int t;
			std::cin >> t;
			edges[t - 1].isend = 1;
		}
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
		/*
		for (int i = 1; i <= n; i++) {
			std::cout << i << std::endl;
			for (int j = next[i]; j; j = next[j])
				std::cout << num[j] << ' ';
			std::cout << std::endl;
		}
		*/
		memset(&(edgedone[0]), 255, sizeof(int) * edgedone.size());
		for (int i = 0; i < bcc.size(); i++) {
			auto &bb = bcc[i];
			int set = i + cutn + 1;
			for (int j : bb) {
				//std::cout << set << ' ' << oo::val[j] << ' ' << oo::num[j] << oo::num[j ^ 1] << std::endl;
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
	parse::parsetest();

	//std::cin >> n >> m;
	n = parse::points.size();
	m = parse::edges.size();

	resizevector(n, m);
	etot = n + 1;
	if ((etot & 1) == 0) etot++;
	for (int i = 1; i <= n; i++)
		tail[i] = i;
	for (int i = 1; i <= n; i++) {
		vertex_id[i] = strings.size();

		//strings.push_back("");
		//std::cin >> strings.back();
		strings.push_back(parse::points[i - 1]);

	}
	int sttot = 0, edtot = 0, lasts, laste;
	for (int i = 1; i <= m; i++) {
		int u, v, s = strings.size();

		//strings.push_back("");
		//std::cin >> u >> v >> strings.back();
		u = parse::edges[i - 1].x;
		v = parse::edges[i - 1].y;
		strings.push_back(parse::edges[i - 1].id);

		AddEdge(u, v, s);
		AddEdge(v, u, s);

		starts[etot] = starts[etot - 1] = parse::edges[i - 1].isstart;
		ends[etot] = ends[etot - 1] = parse::edges[i - 1].isend;
	}
	int sn, en;

	//std::cin >> sn;
	sn = parse::S.size();

	//for (; sn--; ) {
	//	int t;
	//	std::cin >> t;
	for (auto t : parse::S) {

		starts[t] = 1;
		lasts = t;
		sttot++;
	}

	//std::cin >> en;
	en = parse::T.size();

	//for (; en--; ) {
	//	int t;
	//	std::cin >> t;
	for (auto t : parse::T) {

		ends[t] = 1;
		laste = t;
		edtot++;
	}
	//std::cout << sttot << ' ' << edtot << ' ' << lasts << ' ' << laste << '\n';
	if (1 == edtot && sttot == 1 && lasts == laste)
		check = lasts;
}

int main() {
	//freopen("input.txt", "r", stdin);
	freopen("result.txt", "w", stdout);
	std::ios::sync_with_stdio(0);
	int n, m, check = -1;
	input(n, m, check);

	//if srart and end is one same point, return it
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
	//for (auto &i : ans)
	//	ansset.insert(strings[i]);
	for (auto &i : ans)
		std::cout << strings[i] << '\n';

	return 0;
}