#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;


int main(int argc, char* argv[])
{
	registerTestlibCmd(argc, argv);
	int n = inf.readInt();
	int m = inf.readInt();
	vector<int> model_ans(n);
	vector<int> contestant_ans(n);
	for(int i=0; i<n; i++) {
        contestant_ans[i] = ouf.readInt();
	}
	for(int i=0; i<n; i++) {
        model_ans[i] = ans.readInt();
	}
    bool all_correct = true;
    bool final_correct = true;
    for(int i=0; i<n; i++) {
        if(contestant_ans[i]==0 or contestant_ans[i]<-1 or contestant_ans[i]>m) {
            quitf(_wa, "wa\n");
        }
    }
    for(int i=0; i<n; i++) {
        if(contestant_ans[i] != model_ans[i]) {
            all_correct = false;
        }
        if((contestant_ans[i]==-1) ^ (model_ans[i]==-1)) {
            final_correct = false;
        }
    }
    if(!final_correct) {
        quitf(_wa, "wa\n");
    }
    if(all_correct) {
        quitf(_ok, "ac\n");
    } else {
        quitp(0.5, "-1 correct\n");
    }
	return 0;
}
