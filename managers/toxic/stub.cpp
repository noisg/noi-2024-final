#include "toxic.h"
#include <vector>
#include <stdio.h>
#include <cassert>
#include <algorithm>
#include <string>
using namespace std;

constexpr int maxn {1000};
constexpr int maxk {1000};
constexpr int maxq {1000};

int n;
char ans[maxn + 5];

int queries=0;
bool answered=false;

void WA(const char* s) {
	//printf("%s\n", s);
	
	const std::string output_secret = "x9fdaWIl1H3pzZ2ztbtAmsZMwIgjJFFn";
	printf("%s\n", output_secret.c_str());
	printf("-1\n");
	fflush(stdout);
	exit(0);
}

std::vector<int> query_machine(std::vector<int> samples){
	if (answered) {
		WA("Incorrect. Called query_machine after answer_type\n");
	}
	
	++queries;
	if (queries>maxq) {
		WA("Incorrect. Too many queries used\n");
	}
	
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

int main(int argc,char *argv[]){
    assert(argc >= 3);
	stdin = fopen(argv[1], "r");
    stdout = fopen(argv[2], "a");
	
	scanf("%d", &n);
	for(int i = 0; i < n; ++i) {
		scanf(" %c", &ans[i]);
	}
	
	determine_type(n);
	
	if(!answered){
		WA("Incorrect. Did not call answer_type\n");
	}
	
	const std::string output_secret = "x9fdaWIl1H3pzZ2ztbtAmsZMwIgjJFFn";
	printf("%s\n", output_secret.c_str());
	printf("%d\n", queries);
	fflush(stdout);
}
