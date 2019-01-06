#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;

using ll = long long;
using db = double;
using vi = vector<int>;
using pii = pair<int,int>;
#define pb push_back
#define fir first
#define sec second

using json = nlohmann::json;

namespace parse{
	struct edge{
		int x, y;
		string id;
	};
	void parse(vector<string> &points, vector<Edge> &edges, vector<string> starts, vectoe<string> ends);
}