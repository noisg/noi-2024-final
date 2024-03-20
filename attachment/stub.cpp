#include "toxic.h"
#include <vector>
#include <stdio.h>
#include <algorithm>
using namespace std;

constexpr int maxn {1000};
constexpr int maxk {1000};
constexpr int maxq {2000};

int n;
char ans[maxn + 5];

int queries=0;
bool answered=false;

void WA(const char* s) {
	printf("%s\n", s);
	fflush(stdout);
	exit(0);
}

std::vector<int> query_machine(std::vector<int> samples){
	if (answered) {
		WA("Incorrect. Called query_machine after answer_type\n");
	}
	
	++queries;
	
	if (samples.size() > maxk) {
		WA("Incorrect. Too many samples passed to query_machine\n");
	}
	
	vector<int> cur;
	for (int i: samples) {
		if (i < 0 || i >= n) {
			WA("Incorrect. Invalid species number passed to query_machine\n");
		}
		cur.push_back(i);
	}
	
	vector<int> res;
	while (true) {
		vector<int> nw;
		for (int i = 0; i < cur.size(); ++i) {
			bool rem = false;
			if (i != 0 && ans[cur[i]] == 'R' && ans[cur[i - 1]] == 'T') rem = true;
			if (i != cur.size() - 1 && ans[cur[i]] == 'R' && ans[cur[i + 1]] == 'T') rem = true;
			if (!rem) nw.push_back(cur[i]);
		}
		
		if (cur.size() == nw.size()) {
			if (res.empty()) res.push_back(nw.size());
			break;
		}
		res.push_back(nw.size());
		
		swap(cur, nw);
	}
	
	return res;
}

void answer_type(std::vector<char> v){
	if (answered) {
		WA("Incorrect. Called answer_type twice\n");
	}
	
	answered=true;
	if (v.size() != n) {
		WA("Incorrect number of elements returned\n");
	}
	
	for (int i = 0; i < n; ++i) {
		if (v[i] != ans[i]) {
			WA("Incorrect value returned\n");
		}
	}
}

int main() {
	scanf("%d", &n);
	for(int i = 0; i < n; ++i) {
		scanf(" %c", &ans[i]);
	}
	
	determine_type(n);
	
	if(!answered){
		WA("Incorrect. Did not call answer_type\n");
	}
	
	printf("Correct. %d queries used\n", queries);
	fflush(stdout);
}
