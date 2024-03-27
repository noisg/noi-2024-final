#include <bits/stdc++.h>
using namespace std;

#define sf scanf
#define pf printf
#define fi first
#define se second
#define pb push_back
#define all(x) x.begin(),x.end()
typedef long long ll;
typedef pair<int,int> ii;

int main(){
	int c,p;
	sf("%d%d",&c,&p);
	vector<ii> v;
	for(int i=0;i<c;++i){
		int q,s;sf("%d%d",&q,&s);
		v.pb({q,s});
	}
	sort(all(v));
	vector<int> pfx;
	for(int i=0;i<c;++i){
		if(i==0)pfx.pb(v[i].se);
		else pfx.pb(max(pfx.back(),v[i].se));
	}
	ll ans=0;
	for(int i=0;i<p;++i){
		int q,d;sf("%d%d",&q,&d);
		int x=upper_bound(all(v),ii(q,1e9))-v.begin()-1;
		if(x!=-1)ans+=max(0,pfx[x]-d);
	}
	pf("%lld\n",ans);
}
