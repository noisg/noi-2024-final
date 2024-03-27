/*input
4 5
1 2 3
2 3 4
3 4 1
1 4 2
4 2 1
*/
#include<bits/stdc++.h>
using namespace std;
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
typedef tree<int,null_type,less<int>,rb_tree_tag,tree_order_statistics_node_update> indexed_set;
//#pragma GCC target("avx2")
//order_of_key #of elements less than x
// find_by_order kth element
using ll=long long;
using ld=long double;
using pii=pair<int,int>;
#define f first
#define s second
#define pb push_back
#define REP(i,n) for(int i=0;i<n;i++)
#define REP1(i,n) for(int i=1;i<=n;i++)
#define FILL(n,x) memset(n,x,sizeof(n))
#define ALL(_a) _a.begin(),_a.end()
#define sz(x) (int)x.size()
#define SORT_UNIQUE(c) (sort(c.begin(),c.end()),c.resize(distance(c.begin(),unique(c.begin(),c.end()))))
const ll maxn=5e5+5;
const ll maxlg=__lg(maxn)+2;
const ll INF64=4e18;
const int INF=0x3f3f3f3f;
const ll MOD=998244353;
const ld PI=acos(-1);
const ld eps=1e-4;
#define lowb(x) x&(-x)
#define MNTO(x,y) x=min(x,(__typeof__(x))y)
#define MXTO(x,y) x=max(x,(__typeof__(x))y)
template<typename T>
ostream& operator<<(ostream& out,vector<T> V){
    REP(i,sz(V)) out<<V[i]<<((i!=sz(V)-1)?" ":"\n");
    return out;
}
template<typename T1,typename T2>
ostream& operator<<(ostream& out,pair<T1,T2> P){
    out<<P.f<<' '<<P.s;
    return out;
}

ll mult(ll a,ll b){
    return a*b%MOD;
}
ll mypow(ll a,ll b){
    a%=MOD;
    if(a==0) return 0;
    if(b<=0) return 1;
    ll res=1LL;  
    while(b){
        if(b&1) res=(res*a)%MOD;
        a=(a*a)%MOD;
        b>>=1;
    }
    return res;
}
vector<pii> v[maxn];
int to[maxn];
int col[maxn];
vector<int> g[maxn];
void dfs(int u){
    for(int x:g[u]){
        if(!col[x]){
            col[x]=3-col[u];
            dfs(x);
        }
    }
}
int main(){
    ios::sync_with_stdio(false),cin.tie(0);
    int n,m;
    cin>>n>>m;
    REP(i,m){
        int a,b,w;
        cin>>a>>b>>w;
        v[a].pb({w,b});
        v[b].pb({w,a});
    }
    ll ans=0;
    vector<int> st;
    REP1(i,n){
        sort(ALL(v[i]));
        MXTO(ans,v[i][0].f);
        to[i]=v[i][0].s;
        if(to[v[i][0].s]==i) continue;
        g[i].pb(v[i][0].s);
        g[v[i][0].s].pb(i);
    }
    cout<<ans<<'\n';
    REP1(i,n){
        if(!col[i]){
            col[i]=1;
            dfs(i);
        }
        if(col[i]==1) cout<<'B';
        else cout<<'D';
    }
}   