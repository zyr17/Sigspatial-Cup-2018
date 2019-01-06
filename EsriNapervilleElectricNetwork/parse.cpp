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

int main() {
	
	ios :: sync_with_stdio(0);
	freopen("input", "w", stdout);
	
	freopen("EsriNapervilleElectricNetwork.json", "r", stdin);
	string s, t;
	while(getline(cin, s))
		t += s;
	
	json j = json :: parse(t);
	map <string, int> disc;
	for(auto i : j["rows"]) {
		//cout << i << '\n';
		disc[i["fromGlobalId"]] = 0;
		disc[i["toGlobalId"]] = 0;
	}
	
	int n = 0;
	for(auto &i : disc)
		i.sec = ++n;
	int m = j["rows"].size();
	cout << n << ' ' << m << '\n';
	for(auto i : disc)
		cout << i.fir << '\n';
	
	for(auto i : j["rows"]) {
		cout << disc[i["fromGlobalId"]] << ' ';
		cout << disc[i["toGlobalId"]] << ' ';
		s = i["viaGlobalId"];
		cout << s << '\n';
	}
	
	vi S;
	for(auto i : j["controllers"]) {
		S.pb(disc[i["globalId"]]);
	}
	vi T;
	freopen("startingpoints.txt", "r", stdin);
	cin.clear();
	while(getline(cin, s)) {
		//cout << s << '\n';
		T.pb(disc[s]);
		//for(auto i : disc)
			//if(i.sec == 2159) {
				//cout << i.fir << '\n';
				//cout << (i.fir == s) << '\n';
				//cout << s.size() << ' ' << i.fir.size() << '\n';
			//}
	}
	
	cout << S.size() << '\n';
	for(int x : S) cout << x << '\n';
	cout << T.size() << '\n';
	for(int x : T) cout << x << '\n';
	
	return 0;
}
