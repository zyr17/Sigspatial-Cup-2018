#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using db = double;
using vi = vector<int>;
using pii = pair<int,int>;
#define pb push_back
#define fir first
#define sec second

const int N = 1e5L + 11;

string vertex_id[N], edge_id[N];

struct edge {
	int u, v;
}e[N];
int ecnt = 2;
int first[N], nxt[N];
void add(int u, int v) {
	e[ecnt] = {u, v};
	nxt[ecnt] = first[u]; first[u] = ecnt ++;
}

int dfn[N], low[N]; int cnt = 0;
bool cut[N]; stack <int> sta; vector <vi> bcc;
int tarjan(int x, int pe) {
	dfn[x] = low[x] = ++ cnt; int ecnt = 0;
	for(int e = first[x]; e; e = nxt[e]) if(e != pe) {
		int i = ::e[e].v;
		if(dfn[i] < dfn[x]) sta.push(e);
		if(!dfn[i]) {
			low[x] = min(low[x], tarjan(i, e ^ 1));
			++ ecnt;
			if(low[i] >= dfn[x]) {
				bcc.pb({});
				do {
					bcc.back().pb(sta.top()); sta.pop();
				}while(bcc.back().back() != e);
				if(pe) cut[x] = 1;
			}
		} else
			low[x] = min(low[x], dfn[i]);
	}
	if(!pe && ecnt > 1) cut[x] = 1;
	return low[x];
}

vi te[N];
int MAP[N];

bool ans[N];
bool dfs1(vi e[], int x, int p, int t, vi &v) {
	v.pb(x);
	if(x == t) return 1;
	
	for(int i : e[x]) if(i != p)
		if(dfs1(e, i, x, t, v))
			return 1;
	
	v.pop_back();
	return 0;
}
void solve1(int n, vi e[], vi S, vi T) {
	fill(ans, ans + n + 1, 0);
	for(int s : S) for(int t : T) {
		vi v; dfs1(e, s, 0, t, v);
		for(int i : v)
			ans[i] = 1;
	}
}

bool inS[N], inT[N];

void dfs2(vi e[], int x, int p, int &c1, int &c2) {
	c1 += inS[x]; c2 += inT[x];
	for(int i : e[x]) if(i != p)
		dfs2(e, i, x, c1, c2);
}

void solve2(int n, vi e[], vi S, vi T) {
	fill(ans, ans + n + 1, 0);
	fill(inS, inS + n + 1, 0);
	fill(inT, inT + n + 1, 0);
	for(int x : S) inS[x] = 1;
	for(int x : T) inT[x] = 1;
	for(int x = 1; x <= n; x ++) {
		int a = 0, b = 0, c = 0;
		for(int i : e[x]) {
			int c1 = 0, c2 = 0;
			dfs2(e, i, x, c1, c2);
			a += (c1 > 0);
			b += (c2 > 0);
			c += (c1 > 0) && (c2 > 0);
		}
		
		if(inS[x]) a ++;
		if(inT[x]) b ++;
		if(inS[x] && inT[x]) c ++;
		
		if(inS[x] && inT[x] || (a > 0 && b > 0 && (a - c > 0 || b - c > 0)))
			ans[x] = 1;
	}
}

int as = 0, at = 0;
int cs[N], ct[N];

void dfs3(vi e[], int x, int p) {
	cs[x] = inS[x]; ct[x] = inT[x];
	int a = 0, b = 0, c = 0;
	for(int i : e[x]) if(i != p) {
		dfs3(e, i, x);
		cs[x] += cs[i]; ct[x] += ct[i];
		if(cs[i]) a ++;
		if(ct[i]) b ++;
		if(cs[i] && ct[i]) c ++;
	}
	
	if(inS[x]) a ++;
	if(inT[x]) b ++;
	if(inS[x] && inT[x]) c ++;
	
	if(as - inS[x]) a ++;
	if(at - inT[x]) b ++;
	if((as - inS[x]) && (at - inT[x])) c ++;
	
	if(inS[x] && inT[x] || (a > 0 && b > 0 && (a - c > 0 || b - c > 0)))
		ans[x] = 1;
}

void solve3(int n, vi e[], vi S, vi T) {
	fill(ans, ans + n + 1, 0);
	fill(inS, inS + n + 1, 0);
	fill(inT, inT + n + 1, 0);
	for(int x : S) inS[x] = 1;
	for(int x : T) inT[x] = 1;
	as = S.size(); at = T.size();
	dfs3(e, 1, 0);
}

vi MAPV[N];
int MAPE[N];

bool ansV[N];

int main() {
	
	ios :: sync_with_stdio(0);
	freopen("input.txt", "r", stdin);
	int n, m; cin >> n >> m;
	for(int i = 1; i <= n; i ++)
		cin >> vertex_id[i];
	for(int i = 1; i <= m; i ++) {
		int u, v;
		cin >> u >> v >> edge_id[i];
		add(u, v); add(v, u);
	}
	
	for(int i = 1; i <= n; i ++)
		if(!dfn[i]) tarjan(i, 0);
	
	//for(int i = 1; i <= n; i ++)
		//if(cut[i]) cout << i << '\n';
	
	int tcnt = 0;
	for(int i = 1; i <= n; i ++)
		if(cut[i]) MAP[i] = ++ tcnt;
	int p = 0;
	for(auto c : bcc) {
		unordered_set <int> s;
		for(int e : c)
			s.insert({::e[e].u, ::e[e].v});
		int x = ++ tcnt;
		MAPE[p ++] = x;
		for(int i : s) {
			MAPV[i].pb(x);
			if(cut[i]) {
				te[MAP[i]].pb(x);
				te[x].pb(MAP[i]);
			} else
				MAP[i] = x;
		}
	}
	
	
	//cout << tcnt << '\n';
	//for(int i = 1; i <= tcnt; i ++)
		//for(int j : te[i])
			//if(i < j) cout << i << ' ' << j << '\n';
	
	vi S, T;
	{
		int n; cin >> n;
		while(n --) {
			int x; cin >> x;
			S.pb(x);
		}
	}
	{
		int n; cin >> n;
		while(n --) {
			int x; cin >> x;
			T.pb(x);
		}
	}
	
	for(int &x : S)
		x = MAP[x];
	for(int &x : T)
		x = MAP[x];
	
	solve3(tcnt, te, S, T);
	for(int i = 1; i <= tcnt; i ++)
		for(int x : MAPV[i]) if(ans[x])
			ansV[i] = 1;
	set <string> v;
	for(int i = 1; i <= tcnt; i ++)
		if(ansV[i]) v.insert(vertex_id[i]);
	//for(string s : v) cout << s << '\n';
	//v.clear();
	
	p = 0;
	for(auto c : bcc) {
		if(ans[MAPE[p]]){
			for(int e : c) {
				//cout << "------\n";
				//cout << ::e[e].u << ' ' << ::e[e].v << '\n';
				//cout << e << ' ' << edge_id[e / 2] << '\n';
				v.insert(edge_id[e / 2]);
			}
		}
		p ++;
	}
	std::cout << std::endl;
	for(string s : v) cout << s << '\n';
	
	//srand(25252);
	
	//int ss = 1, tt = 1;
	
	//for(int i = 0; i < 5; i ++) {
	
		//vector <int> v;
		//for(int i = 1; i <= tcnt; i ++)
			//v.pb(i);
		//random_shuffle(v.begin(), v.end());
		//S = vi(v.begin(), v.begin() + ss);
		//T = vi(v.end() - tt, v.end());
	
		//auto time_begin = clock();
		
		//solve1(tcnt, te, S, T);
		//cout << tcnt - count(ans + 1, ans + tcnt + 1, 1) << ' ' << double(tcnt - count(ans + 1, ans + tcnt + 1, 1)) / tcnt  << '\n';
		
		//int a = 0;
		//for(int i = 1; i <= n; i ++)
			//if(ans[MAP[i]]) a ++;
		//int b = 0;
		//int p = 0;
		//for(auto c : bcc) {
			//if(ans[MAPE[p]])
				//b += c.size();
			
			//p ++;
		//}
		//cout << n - a << ' ' << double(n-a) / n << '\n';
		//cout << m - b << ' ' << double(m-b) / m << '\n';
		
		//auto time_end = clock();
		
		//cout << double(time_end - time_begin) / CLOCKS_PER_SEC << endl;
		
		//time_begin = clock();
		
		//solve2(tcnt, te, S, T);
		//cout << count(ans + 1, ans + tcnt + 1, 1) << '\n';
		
		//time_end = clock();
		
		//cout << double(time_end - time_begin) / CLOCKS_PER_SEC << endl;
		
		//time_begin = clock();
		
		//solve3(tcnt, te, S, T);
		//cout << count(ans + 1, ans + tcnt + 1, 1) << '\n';
		
		//time_end = clock();
		
		//cout << double(time_end - time_begin) / CLOCKS_PER_SEC << endl;
	//}
	
	return 0;
}
