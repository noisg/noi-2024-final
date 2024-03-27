#include "toxic.h"
#include <bits/stdc++.h>
using namespace std;

constexpr int B {50};
constexpr int maxsz {36};
constexpr int frontsz {26};

void determine_type(int n) {
    vector<char> ans(n, 'T');
    
    // phase 1: find a toxic bacteria
    int toxic = -1, start = 0;
    {
        vector<int> qry;
        for(int i = 0; i < 500; i++) {
            qry.push_back(0);
        }
        for(int i = 0; i < n / 2; i++) {
            qry.push_back(i);
        }
        auto res = query_machine(qry);
        
        if (res.back() == qry.size()) {
            // [0, 499] all same type
            start = 500;
            
            // first check if 999 is toxic
            auto res = query_machine({0, 0, n - 1});
            if (res.back() == 2) {
                toxic = 0;
            } else if (res.back() == 1) {
                toxic = n - 1;
                for (int i = 0; i < n / 2; ++i) {
					// if 0 is regular, [1, 499] also regular
                    ans[i] = 'R';
                }
            } else {
                qry.clear();
                for(int i = 0; i < 501; i++) {
                    qry.push_back(0);
                }
                for(int i = n / 2; i < n - 1; i++) {
                    qry.push_back(i);
                }
                auto res = query_machine(qry);
                
                if (res.back() > 500) {
                    toxic = 0;
                } else {
					// first toxic i takes (i - 500) + 501 turns
                    toxic = res.size() - 501 + n / 2;
                    for (int i = 0; i < n / 2; ++i) { 
						// if 0 is regular, [1, 499] also regular
                        ans[i] = 'R'; 
                    } 
                } 
            }            
        } else { 
            // at least one regular and toxic in [0, 499]
            if (res.back() > 500) {
				// 0 didn't die - it is toxic
                toxic = 0;
            } else {
				// first toxic i takes i + 500 turns to kill less than it
                toxic = res.size() - 500;
            }
		}
	} 

	// phase 2: split into n / B groups 
	// each query looks like 
	// (26) (T 1) (T 2) (T 2 T 2) (T 2 T 2 T 2 T 2) ... (T 3) 
	// (T 4) (T 4 T 4) (T 4 T 4 T 4 T 4) ... (T 5) ... T (27)
    for (int low = start; low < n; low += B) {
        vector<int> qry;
		int cur = 0;
		int rem = 1000;
		
		for (int i = 0; i < frontsz; ++i) {
			// put stuff in front
			qry.push_back(low + B - 2);
		}
		
		for (int i = 1; i <= maxsz; ++i){
			// i = number of copies of cur
			for (int p = 0; p < ((i % 2 == 1) ? 1 : 10); ++p) {
				// if i is even, we can have "multiples"
				// 2^p copies of (T i)
				int num = i * (1 << p) + (1 << p);
				if (num > maxsz) {
					break;
				}
				rem -= num;
				for (int j = 0; j < (1 << p); ++j) {
					qry.push_back(toxic);
					for (int x = 0; x < i; ++x) {
						qry.push_back(low + cur);
					}
				}
				++cur;
			}
		}
		qry.push_back(toxic);
		
		for (int i = 0; i < frontsz + 1; ++i) {
			// put stuff behind
			qry.push_back(low + B - 1);
		}
		
		assert(qry.size() <= 1000);
		auto res = query_machine(qry);
		
		int fbcnt = 0; // count of front/back that are regular
		if(res.size() == frontsz + 1){
			// if we last frontsz + 1 turns, back value is regular
			ans[low + B - 1] = 'R';
			res.pop_back();
			res[frontsz - 1]++; // "fix" this value
			++fbcnt;
		}
		if(res.size() == frontsz && res[frontsz - 2] != res[frontsz - 1]){
			// if we last frontsz turns and res decreased by 1, front value is regular
			ans[low + B - 2] = 'R';
			res.pop_back();
			++fbcnt;
		}
		
		res.emplace(res.begin(), qry.size());
		for (int i = 0; i < res.size(); ++i) {
			// remove "contribution" of front/back
			res[i] -= fbcnt * (res.size() - i);
		}
		res.push_back(res.back());
		
		vector<int> diff;
		for (int i = 1; i < res.size(); ++i) {
			diff.push_back(res[i - 1] - res[i]);
		}
		diff.push_back(0);
		
		//# of ppl who died
		cur = 0;
		for (int i = 0; i < diff.size(); ++i) {
			if (diff[i] == 0) break;
			if (diff[i] % 2 == 1) {
				// for odd values, if diff is odd --> regular
				ans[low + cur] = 'R';
			}
			++cur;
			for (int p = 0; p < 10; ++p) {
				int num = (i + 1) * 2 * (1 << p) + (1 << p);
				if (num > maxsz) {
					break;
				}
				if ((diff[i] - diff[i + 1] - diff[i] % 2 - diff[i + 1] % 2) & (1 << (p + 1))) {
					ans[low + cur] = 'R';
				}
				++cur;
			}
		}
    } 
    answer_type(ans); 
}
