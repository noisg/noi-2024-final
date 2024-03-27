#include <bits/stdc++.h>
#define low(i) (i<<(__builtin_clz(i)-31+n_bits))-(1<<n_bits)
#define high(i) ((i+1)<<(__builtin_clz(i)-31+n_bits))-(1<<n_bits)-1
using namespace std;
const int n_max = 3e5;
int n_global;
const int n_bits=20;
const int inf = 1e9;
int arr[1<<(n_bits+1)];
int lazyadd[1<<(n_bits+1)];

struct segtree {
    segtree(){}
    void build(vector<int> v) {
        assert(v.size()<(1<<n_bits));
        for(int i=0; i<(1<<n_bits); i++) {
            arr[i+(1<<n_bits)] = (i<(int)v.size() ? v[i] : inf);
        }
        for(int i=(1<<n_bits)-1; i>=1; i--) {
            arr[i] = min(arr[2*i], arr[2*i+1]);
        }
        for(int i=0; i<(1<<(n_bits+1)); i++) {
            lazyadd[i] = 0;
        }
    }
    int value(int node) {
        arr[node] += lazyadd[node];
        if(node<(1<<n_bits)) {
            lazyadd[2*node] += lazyadd[node];
            lazyadd[2*node+1] += lazyadd[node];
        }
        lazyadd[node] = 0;
        return arr[node];
    }
    void update(int node, int left, int right, int change) {
        if(right>=high(node) && left<=low(node)) {
            lazyadd[node] += change;
        } else if(right<low(node) || left>high(node)) {
            return;
        } else {
            update(2*node, left, right, change);
            update(2*node+1, left, right, change);
            arr[node] = min(value(node*2), value(node*2+1));
        }
    }

    void decr(int left, int right) {
        update(1, left, right, -1);
    }

    int find_zero(int node) {
        if(value(node)!=0) {
            return -1;
        }
        if(node >= (1<<n_bits)) {
            return arr[node]==0 ? node-(1<<n_bits) : -1;
        }
        int x = find_zero(node*2);
        if(x!=-1) return x;
        return find_zero(node*2+1);
    }
};

int a[n_max];
int b[n_max];

vector<int> get_topo_order(vector<vector<int>> out_edge, vector<int> in_deg, int n) {
    vector<int> ans;
    queue<int> b;
    for(int i=0; i<n; i++) {
        if(in_deg[i]==0) {
            b.push(i);
        }
    }
    while(!b.empty()) {
        int next = b.front();
        ans.push_back(next);
        b.pop();
        for(int i: out_edge[next]) {
            in_deg[i]--;
            if(in_deg[i] == 0) {
                b.push(i);
            }
        }
    }
    assert(ans.size() == n);
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(0);
    int n,m; scanf("%d%d",&n,&m);
    segtree st;
    st.build(vector<int>(n, n-1)); // initialize all to n-1
    vector<pair<int,int>> edgelist(m);
    vector<vector<int>> out_edge(n);
    vector<int> in_deg(n);
    for(int i=0; i<m; i++) {
        int x,y; scanf("%d%d",&x,&y); x--; y--;
        edgelist[i] = make_pair(x,y);
        out_edge[x].push_back(y);
        in_deg[y]++;
    }
    vector<int> topo_order = get_topo_order(out_edge, in_deg, n);
    vector<int> inv_topo_order(n);
    for(int i=0; i<n; i++) {
        inv_topo_order[topo_order[i]] = i;
    }
    for(pair<int,int> &p: edgelist) {
        p.first = inv_topo_order[p.first];
        p.second = inv_topo_order[p.second];
    }

    for(int i=0; i<n; i++) {
        a[i] = n;
        b[i] = -1;
    }
    // a[i] = min outgoing edge from i
    // b[i] = max incoming edge from i
    vector<int> turn_zero(n, -2); // -1 -> incomparable, else set to the value
    for(int i=0; i<m; i++) {
        int x = edgelist[i].first;
        int y = edgelist[i].second;
        assert(x<y);
        if(a[x]>y) {
            st.decr(y,a[x]-1);
            a[x] = y;
        }
        if(b[y]<x) {
            st.decr(b[y]+1, x);
            b[y] = x;
        }
        int next_zero;
        do {
            next_zero = st.find_zero(1);
            if(next_zero != -1) {
                turn_zero[next_zero] = i;
            }
            st.update(1, next_zero, next_zero, 1<<20);
        } while(next_zero != -1);
    }
    for(int i=0; i<n; i++) {
        printf("%d ", turn_zero[inv_topo_order[i]]+1);
    }
    printf("\n");
}
