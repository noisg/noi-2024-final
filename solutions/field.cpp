#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <functional>
#include <queue>
using namespace std;
 
#define MAXN 400
#define MAXQ 200'000
#define MAXM 1'000'000'001LL
 
typedef long long ll;
typedef pair<ll,ll> pl;
typedef tuple<ll,ll,ll> event;
 
const ll INF = 3e18;
 
ll N, T, Q, A[MAXN+5], B[MAXN+5], C[MAXN+5], D[MAXN+5], M[MAXQ+5], Qx[MAXQ+5], Qy[MAXQ+5], ans[MAXQ+5];
ll Z[2*MAXN+10][2*MAXN+10], S[2*MAXN+10][2*MAXN+10], X_region_cnt, Y_region_cnt;
ll dist[2*MAXN+10][2*MAXN+10][4];
ll global_past_moves, global_ac, global_delta_ac, global_delta2_ac, fc;
ll region_past_moves[2*MAXN+10][2*MAXN+10], region_ac[2*MAXN+10][2*MAXN+10];
ll region_delta_ac[2*MAXN+10][2*MAXN+10], region_delta2_ac[2*MAXN+10][2*MAXN+10];
bool region_is_finished[2*MAXN+10][2*MAXN+10], vis[2*MAXN+10][2*MAXN+10][4];
vector <ll> X_divide, Y_divide;
priority_queue < event, vector<event>, greater<event> > event_queue;
vector <event> event_vec;
 
/*      2 -----------> 3
 *      ^              ^
 *   +y |              | 
 *      |              |
 *      0 -----------> 1
 *             +x
 *
 *   first run
 *   - propagate:       (moves, [region i, region j, corner], unused)
 *
 *   second run
 *   - finish:          (moves, [region i, region j, type = 1], unused)
 *   - update_delta:    (moves, [region i, region j, type = 2], change)
 *   - query:           (moves, [query_id, type = 3], unused)
 *
 *   items in [] are encoded as a single long long
 *       region i (12) | region j (12) | corner (40)
 *       region i (12) | region j (12) | type (40)
 *       query_id (24) | type (40)
 */
 
inline void sort_remove_duplicate(vector <ll> &_V)
{
	sort(_V.begin(), _V.end());
	_V.resize(unique(_V.begin(), _V.end()) - _V.begin());
}
 
inline ll get_index(vector <ll> &_V, ll _X)
{
	return (upper_bound(_V.begin(), _V.end(), _X) - _V.begin() - 1);
}
 
void fast_forward(ll &_past_moves, ll &_ac, ll &_delta_ac, ll &_delta2_ac, ll &_cur_moves)
{
	ll _delta_m = _cur_moves - _past_moves;
	_ac += _delta_m * _delta_ac + _delta_m * (_delta_m+1) / 2 * _delta2_ac;
	_delta_ac += _delta_m * _delta2_ac;
	_past_moves = _cur_moves;
}
 
void propagate_dist(ll _next_i, ll _next_j, ll _next_c, ll _next_d)
{
	if (_next_i >= 0 && _next_i < X_region_cnt
	    && _next_j >= 0 && _next_j < Y_region_cnt
	    && S[_next_i][_next_j] == 0 && dist[_next_i][_next_j][_next_c] > _next_d)
	{
		dist[_next_i][_next_j][_next_c] = _next_d;
		event_queue.emplace(_next_d, _next_i + (_next_j << 12) + (_next_c << 24), 0);
	}
}
 
void queue_delta(vector<pl> &_delta_list, ll _limit, ll _moves, ll _l, ll _h, ll _base)
{
	if (_moves < _limit) _delta_list.emplace_back(_moves, _base);
	if (_moves + min(_l, _h) < _limit) _delta_list.emplace_back(_moves + min(_l, _h), -_base);
	if (_moves + max(_l, _h) < _limit) _delta_list.emplace_back(_moves + max(_l, _h), -_base);
}
 
int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	
	cin >> N >> T >> Q;
	
	for (ll i = 0; i < N; i++)
	{
		cin >> A[i] >> B[i] >> C[i] >> D[i];
	}
	
	for (ll q = 0; q < Q; q++)
	{
	    if (T == 1) cin >> Qx[q] >> Qy[q];
		else if (T == 2) cin >> M[q];
	}
	
	// constructing a new grid
	
	X_divide.push_back(-MAXM);
	X_divide.push_back(0);
	X_divide.push_back(MAXM+1);
	Y_divide.push_back(-MAXM);
	Y_divide.push_back(0);
	Y_divide.push_back(MAXM+1);
	
	for (ll i = 0; i < N; i++)
	{
		X_divide.push_back(A[i]);
		X_divide.push_back(B[i]+1);
		Y_divide.push_back(C[i]);
		Y_divide.push_back(D[i]+1);
	}
	
	sort_remove_duplicate(X_divide);
	sort_remove_duplicate(Y_divide);
	
	X_region_cnt = X_divide.size()-1;
	Y_region_cnt = Y_divide.size()-1;
	
	// prefix sums for blocking regions
	
	memset(Z, 0, sizeof Z);
   
	for (ll i = 0; i < N; i++)
	{
		ll i1 = get_index(X_divide, A[i]);
		ll i2 = get_index(X_divide, B[i]+1);
		ll j1 = get_index(Y_divide, C[i]);
		ll j2 = get_index(Y_divide, D[i]+1);
		
		Z[i1][j1]++;
		Z[i2][j2]++;
		Z[i2][j1]--;
		Z[i1][j2]--;
	}
	
	for (ll i = 0; i < X_region_cnt; i++)
	{
		for (ll j = 0; j < Y_region_cnt; j++)
		{
			S[i][j] = Z[i][j] + (i > 0 ? S[i-1][j] : 0)
			                  + (j > 0 ? S[i][j-1] : 0)
			                  - (i > 0 && j > 0 ? S[i-1][j-1] : 0);
		}
	}
	
	// init variables
	
	global_past_moves = 0;
	global_ac         = 0;
	global_delta_ac   = 0;
	global_delta2_ac  = 0;
	fc = 0;
	
	for (ll i = 0; i < X_region_cnt; i++)
	{
		for (ll j = 0; j < Y_region_cnt; j++)
		{
			region_past_moves[i][j]  = 0;
			region_ac[i][j]          = 0;
			region_delta_ac[i][j]    = 0;
			region_delta2_ac[i][j]   = 0;
			region_is_finished[i][j] = false;
			
			for (ll k = 0; k < 4; k++)
			{
				dist[i][j][k] = INF;
				vis[i][j][k]  = false;
			}
		}
	}
 
	// propagation phase
 
	dist[get_index(X_divide, 0)][get_index(Y_divide, 0)][0] = 0;
	event_queue.emplace(0, get_index(X_divide, 0) + (get_index(Y_divide, 0) << 12) + 0, 0);
	
	while (!event_queue.empty())
	{
		event cur_event = event_queue.top();
		event_queue.pop();
		
		ll event_moves = get<0>(cur_event);
		ll event_ri = (get<1>(cur_event) << 52) >> 52;
		ll event_rj = (get<1>(cur_event) << 40) >> 52;
		ll event_corner = get<1>(cur_event) >> 24;
		if (vis[event_ri][event_rj][event_corner]) continue;
		vis[event_ri][event_rj][event_corner] = true;
		
		ll length = X_divide[event_ri+1] - X_divide[event_ri];
		ll height = Y_divide[event_rj+1] - Y_divide[event_rj];
		
		// propagate to adjacent corners (x-axis)
		
		propagate_dist(event_ri, event_rj, event_corner^1, event_moves+length-1);
		
		if (event_corner & 1) propagate_dist(event_ri+1, event_rj, event_corner^1, event_moves+1);
		else                  propagate_dist(event_ri-1, event_rj, event_corner^1, event_moves+1);
		
		// propagate to adjacent corners (y-axis)
		
		propagate_dist(event_ri, event_rj, event_corner^2, event_moves+height-1);
		
		if (event_corner & 2) propagate_dist(event_ri, event_rj+1, event_corner^2, event_moves+1);
		else                  propagate_dist(event_ri, event_rj-1, event_corner^2, event_moves+1);
	}
	
	// query handling phase for T == 1
	
	if (T == 1)
	{
	    for (ll q = 0; q < Q; q++)
	    {
	        ll ri = get_index(X_divide, Qx[q]);
	        ll rj = get_index(Y_divide, Qy[q]);
	        
	        if (S[ri][rj] == 0)
	        {
	            ans[q] = INF;
	            for (ll k = 0; k < 4; k++)
	            {
	                ll corner_x = (k & 1) ? X_divide[ri+1] - 1 : X_divide[ri];
	                ll corner_y = (k & 2) ? Y_divide[rj+1] - 1 : Y_divide[rj];
	                if (dist[ri][rj][k] < INF) ans[q] = min(ans[q], dist[ri][rj][k] + abs(corner_x - Qx[q]) + abs(corner_y - Qy[q]));
	            }
	            if (ans[q] == INF) ans[q] = -1;
	        }
	        else
	        {
	            ans[q] = -1;
	        }
	        
	        cout << ans[q] << '\n';
	    }
	    
	    return 0;
	}
	
	// event creation phase
	
	for (ll q = 0; q < Q; q++)
	{
		event_vec.emplace_back(M[q], q + (3LL << 24), 0);
	}
	
	for (ll i = 0; i < X_region_cnt; i++)
	{
		for (ll j = 0; j < Y_region_cnt; j++)
		{
			if (S[i][j] != 0) continue;
			
			ll length = X_divide[i+1] - X_divide[i];
			ll height = Y_divide[j+1] - Y_divide[j];
			
			// create finish event
			
			ll finish_moves_min = INF;
			
			for (ll c = 0; c < 2; c++)
			{
				ll finish_moves_cand = (length + height - 2 + dist[i][j][c] + dist[i][j][c^3]) / 2;
				finish_moves_min = min(finish_moves_min, finish_moves_cand);
			}
			
			if (finish_moves_min <= MAXM)
			{
			    event_vec.emplace_back(finish_moves_min, i + (j << 12) + (1LL << 24), 0);
			}
			
			// list out update_delta changes in (moves, change) pairs
			
			vector <pl> delta_changes;
			
			for (ll c = 0; c < 4; c++)
			{
				queue_delta(delta_changes, finish_moves_min, dist[i][j][c], length, height, 1);
				
				if ((c & 1) == 0)
				{
					ll mid = (length + 1 - dist[i][j][c] + dist[i][j][c|1]) / 2;
					ll mid_dist = dist[i][j][c] + mid - 1;
					
					if (mid == 1 || mid == length)
					{
						queue_delta(delta_changes, finish_moves_min, mid_dist, length, height, -1);
					}
					else
					{
						queue_delta(delta_changes, finish_moves_min, mid_dist, mid, height, -1);
						queue_delta(delta_changes, finish_moves_min, mid_dist+1, length-mid, height, -1);
					}
				}
				
				if ((c & 2) == 0)
				{
					ll mid = (height + 1 - dist[i][j][c] + dist[i][j][c|2]) / 2;
					ll mid_dist = dist[i][j][c] + mid - 1;
					
					if (mid == 1 || mid == height)
					{
						queue_delta(delta_changes, finish_moves_min, mid_dist, length, height, -1);
					}
					else
					{
						queue_delta(delta_changes, finish_moves_min, mid_dist, length, mid, -1);
						queue_delta(delta_changes, finish_moves_min, mid_dist+1, length, height-mid, -1);
					}
				}
			}
			
			delta_changes.emplace_back(INF, 0);    // sentinel pair
			
			// merge update_delta changes happening at the same number of moves
			
			sort(delta_changes.begin(), delta_changes.end());
			
			ll cur_m = -1, cur_c = 0;
			
			for (pl dpair : delta_changes)
			{
				if (cur_m != dpair.first)
				{
					if (cur_c != 0) event_vec.emplace_back(cur_m, i + (j << 12) + (2LL << 24), cur_c);
					cur_m = dpair.first;
					cur_c = 0;
				}
				cur_c += dpair.second;
				if (cur_m > min(MAXM, finish_moves_min)) break;
			}
		}
	}
	
	sort(event_vec.begin(), event_vec.end());
	
	// event handling phase
	
	for (event cur_event : event_vec)
	{
		ll event_moves = get<0>(cur_event);
		ll event_type  = get<1>(cur_event) >> 24;
		fast_forward(global_past_moves, global_ac, global_delta_ac, global_delta2_ac, event_moves);
		
		if (event_type == 3)    // query
		{
			ll event_query_id = (get<1>(cur_event) << 40) >> 40;
			ans[event_query_id] = global_ac + fc;
		}
		else
		{
			ll event_ri = (get<1>(cur_event) << 52) >> 52;
			ll event_rj = (get<1>(cur_event) << 40) >> 52;
			fast_forward(region_past_moves[event_ri][event_rj], region_ac[event_ri][event_rj],
			             region_delta_ac[event_ri][event_rj], region_delta2_ac[event_ri][event_rj],
			             event_moves);
			
			if (event_type == 1)    // finish
			{
				if (region_is_finished[event_ri][event_rj]) continue;
				
				global_ac        -= region_ac[event_ri][event_rj];
				global_delta_ac  -= region_delta_ac[event_ri][event_rj];
				global_delta2_ac -= region_delta2_ac[event_ri][event_rj];
				fc += (X_divide[event_ri+1] - X_divide[event_ri]) * (Y_divide[event_rj+1] - Y_divide[event_rj]);
				region_is_finished[event_ri][event_rj] = true;
			}
			else    // event_type == 2: update_delta
			{
				if (region_is_finished[event_ri][event_rj]) continue;
				
				ll event_change = get<2>(cur_event);
				global_ac                            += event_change;
				global_delta_ac                      += event_change;
				global_delta2_ac                     += event_change;
				region_ac[event_ri][event_rj]        += event_change;
				region_delta_ac[event_ri][event_rj]  += event_change;
				region_delta2_ac[event_ri][event_rj] += event_change;
			}
		}
	}
	
	for (ll q = 0; q < Q; q++)
	{
		cout << ans[q] << '\n';
	}
	
	return 0;
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
