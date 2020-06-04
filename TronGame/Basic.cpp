#include <stdio.h>
#include <time.h>

#include <algorithm>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cmath>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using ll = long long;
using ld = long double;
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define all(x) (x).begin(), (x).end()
#define rep3(var, min, max) for (ll(var) = (min); (var) < (max); ++(var))
#define repi3(var, min, max) for (ll(var) = (max)-1; (var) + 1 > (min); --(var))
#define Mp(a, b) make_pair((a), (b))
#define F first
#define S second
#define Icin(s) \
    ll(s);      \
    cin >> (s);
#define Scin(s) \
    ll(s);      \
    cin >> (s);
template <class T>
bool chmax(T& a, const T& b) {
    if (a < b) {
        a = b;
        return 1;
    }
    return 0;
}
template <class T>
bool chmin(T& a, const T& b) {
    if (b < a) {
        a = b;
        return 1;
    }
    return 0;
}
typedef vector<ll> V;
typedef vector<V> VV;
typedef vector<pair<ll, ll>> VP;
ll mod = 1e9 + 7;
ll MOD = 1e9 + 7;
ll INF = 1e9;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
vector<vector<bool>> have_path(20, vector<bool>(30, false));  //通った場所の履歴
vector<vector<pair<int, int>>> player_have_path(
    3);  //各プレイヤーが通った場所の履歴
vector<bool> death(3);
string output[] = {"UP", "DOWN", "LEFT", "RIGHT"};
int dx[] = {0, 0, -1, 1};
int dy[] = {-1, 1, 0, 0};
int N;
int P;
vector<int> X0(3), Y0(3), X1(3), Y1(3);
bool can_move(int y, int x, vector<vector<bool>>& have) {
    if (y >= 20 || y < 0 || x >= 30 || x < 0) {
        return false;
    }
    return !have[y][x];
}
bool can_move_pure(int y, int x) {
    if (y >= 20 || y < 0 || x >= 30 || x < 0) {
        return false;
    }
    return true;
}
vector<vector<int>> BFS_distance(int y, int x) {
    vector<vector<int>> temp(20, vector<int>(30, INF));
    temp[y][x] = 0;
    queue<vector<int>> q;
    q.push({y, x, 0});
    while (!q.empty()) {
        int nowy = q.front()[0];
        int nowx = q.front()[1];
        int nowdis = q.front()[2];
        q.pop();
        for (int way = 0; way < 4; way++) {
            if (can_move(nowy + dy[way], nowx + dx[way], have_path) &&
                temp[nowy + dy[way]][nowx + dx[way]] == INF) {
                temp[nowy + dy[way]][nowx + dx[way]] = nowdis + 1;
                q.push({nowy + dy[way], nowx + dx[way], nowdis + 1});
            }
        }
    }
    return temp;
}
int BFS_num(int y, int x, vector<vector<bool>>& have2, bool k=false) {
    vector<vector<bool>> have(20,vector<bool>(30,true));
    queue<pair<int, int>> q;
    q.push({y, x});
    have[y][x] = false;
    int cnt = 0;
    while (!q.empty()) {
        int nowy = q.front().F;
        int nowx = q.front().S;
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nowy2=nowy+dy[i];
            int nowx2=nowx+dx[i];
            if(k){
                cerr << nowy2 << " " << nowx2 <<" "<<can_move(nowy2, nowx2, have2)<< endl;
            }
            
            //cerr<<"check"<<can_move(nowy2, nowx2, have2)<<" "<<have[nowy2][nowx2]<<endl;
            if (can_move(nowy2, nowx2, have2)&&have[nowy2][nowx2]) {
                if(k){
                cerr << nowy2 << " " << nowx2 <<" "<<can_move(nowy2, nowx2, have2)<<" "<<have[nowy2][nowx2]<< endl;
            }
                cnt++;
                have[nowy2][nowx2] = false;
                q.push({nowy2,nowx2});
            }
        }
    }

    return cnt;
}
int solve() {
    //各プレイヤーごとのボロノイ図の構築
    cerr<<"プレイヤーごとのボロノイ図構築"<<endl;
    vector<vector<int>> Voronoi(20, vector<int>(30, -1));
    vector<vector<int>> Voronoi_reg(20, vector<int>(30, INF - 1));
    for (int i = 0; i < N; i++) {
        if (death[i]) {
            continue;
        }
        vector<vector<int>> BFS_dis = BFS_distance(Y1[i], X1[i]);
        for (int h = 0; h < 20; h++) {
            for (int w = 0; w < 30; w++) {
                if (chmin(Voronoi_reg[h][w], BFS_dis[h][w])) {
                    Voronoi[h][w] = i;
                }
            }
        }
    }
    //進む方向ごとのボロノイ図を構築
    cerr<<"進む方向ごとのボロノイ図を構築"<<endl;
    vector<vector<int>> Voronoi_way(20, vector<int>(30, -1));
    vector<vector<int>> Voronoi_way_reg(20, vector<int>(30, INF - 1));
    for (int way = 0; way < 4; way++) {
        if (!can_move(Y1[P] + dy[way], X1[P] + dx[way], have_path)) {
            continue;
        }
        vector<vector<int>> BFS_dis =BFS_distance(Y1[P] + dy[way], X1[P] + dx[way]);
        for (int h = 0; h < 20; h++) {
            for (int w = 0; w < 30; w++) {
                if (Voronoi[h][w] != P) {
                    continue;
                }
                if (chmin(Voronoi_way_reg[h][w], BFS_dis[h][w])) {
                    Voronoi_way[h][w] = way;
                }
            }
        }
    }
    //いける全点に対して評価をする
    cerr << "いける全点に対して評価をする" << endl;
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();
    vector<vector<int>> BFS_dis = BFS_distance(Y1[P], X1[P]);
    vector<long double> BFS_nums;
    vector<long double> way_score(3);
    for(int i=0;i<N;i++){
        BFS_nums.push_back(BFS_num(Y1[i],X1[i],have_path));
    }
    for(int h=0;h<20;h++){
        for(int w=0;w<30;w++){
            cerr<<endl;
            cerr<<h<<" "<<w<<endl;
            end = chrono::system_clock::now();

            double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
            //cerr<<"time"<< time<<"ms"<<endl;
            if(Voronoi_way[h][w]==-1){
                continue;
            }
            vector<int> order={0,1,2,3};
            long double best_score=0;
           // do{ 
                vector<vector<bool>> temp_have=have_path;
                long double score=0;
                int nowy=h;
                int nowx=w;
                int dist=BFS_dis[nowy][nowx];
                long double dist2=dist;
                while(dist!=0){
                    temp_have[nowy][nowx]=true;
                    for(int j=0;j<4;j++){
                        int way=order[j];
                        if(can_move_pure(nowy+dy[way],nowx+dx[way])){
                            if(BFS_dis[nowy+dy[way]][nowx+dx[way]]==dist-1){
                                nowy = nowy + dy[way];
                                nowx = nowx + dx[way];
                                dist--;
                                break;
                            }
                        }
                    }
                }
                //cerr<<"経路探索終わり"<<endl;
                for(int player=0;player<N;player++){
                    //cerr<<"BFS1前"<<endl;
                    bool k=false;
                    if(h==Y1[P]&&w==X1[P]+2){
                        k=true;
                    }
                    long double num=BFS_num(Y1[player],X1[player],temp_have,k);
                    //cerr<<player<<" "<<num<<endl;
                    num-=dist2;
                    if(num==0){
                        continue;
                    }
                    long double rate =(BFS_nums[player] / num)-1.000;
                    if(player==P){
                        score-=rate;
                    }else{
                        score+=rate;
                    }
                }
                chmax(best_score,score);
            //}while(next_permutation(all(order)));
            way_score[Voronoi_way[h][w]]+=best_score;
        }
    }
    long double way_reg=0;
    int res=0;
    for(int i=0;i<4;i++){
        if(chmax(way_reg,way_score[i])){
            res=i;
        }
    }
    return res;
}
//接敵していない場合、最大利得を目指す
int max_solve() {
    int reg = 0;
    vector<int> res;
    for (int i = 0; i < 4; i++) {
        int nowy = Y1[P] + dy[i];
        int nowx = X1[P] + dx[i];
        if (can_move(nowy, nowx, have_path)) {
            int possible = BFS_num(nowy, nowx, have_path,false);
            if (reg == possible) {
                res.push_back(i);
            } else if (reg < possible) {
                reg = possible;
                res = {i};
            }
        }
    }
    if (res.size() == 1) {
        cerr << "一手目" << endl;
        return res[0];
    }
    cerr << "一手目では決定されず" << endl;
    vector<int> res2;
    int secondreg = 0;
    for (auto i : res) {
        int nowy = Y1[P] + dy[i];
        int nowx = X1[P] + dx[i];
        int tempreg = 0;
        have_path[nowy][nowx] = true;
        for (int j = 0; j < 4; j++) {
            if (can_move(nowy + dy[j], nowx + dx[j], have_path)) {
                chmax(tempreg, BFS_num(nowy + dy[j], nowx + dx[j], have_path));
            }
        }
        cerr<<output[i]<<" "<<i<<" "<<tempreg<<" "<<secondreg<<endl;
        if (secondreg == tempreg) {
            res2.push_back(i);
        } else if (secondreg < tempreg) {
            res2 = {i};
            secondreg=tempreg;
        }
        have_path[nowy][nowx] = false;
    }
    if (res2.size() == 1) {
        cerr << "二手目" << endl;
        return res2[0];
    }
    cerr << "二手目では決定されず" << endl;
    int final_res = 0;
    int final_reg = 4;
    for (auto i : res2) {
        int nowy = Y1[P] + dy[i];
        int nowx = X1[P] + dx[i];
        int num = 0;
        for (int j = 0; j < 4; j++) {
            if (can_move(nowy + dy[j], nowx + dx[j], have_path)) {
                num++;
            }
        }
        cerr << i << " " << num << endl;
        if (chmin(final_reg, num)) {
            final_res = i;
        }
    }
    cerr << "final" << endl;
    return final_res;
}
void death_process(int player) {
    for (auto i : player_have_path[player]) {
        int y = i.F;
        int x = i.S;
        have_path[y][x] = false;
    }
}
bool in_enemy() {
    vector<vector<bool>> temp=have_path;
    queue<vector<int>> q;
    q.push({Y1[P], X1[P]});
    while (!q.empty()) {
        int nowy = q.front()[0];
        int nowx = q.front()[1];
        q.pop();
        for (int way = 0; way < 4; way++) {
            for (int player = 0; player < N; player++) {
                if (P == player||death[player]) {
                    continue;
                }
                if (nowy + dy[way] == Y1[player] &&nowx + dx[way] == X1[player]) {
                    return true;
                }
            }
            if (can_move(nowy + dy[way], nowx + dx[way], temp)) {
                temp[nowy + dy[way]][nowx + dx[way]] = true;
                q.push({nowy + dy[way], nowx + dx[way]});
            }
        }
    }
    return false;
}
int main() {
    while (1) {
        cin >> N >> P;
        for (int i = 0; i < N; i++) {
            cin >> X0[i] >> Y0[i] >> X1[i] >> Y1[i];
            if (X1[i] == -1) {
                if (!death[i]) {
                    death_process(i);
                    death[i] = true;
                }
                continue;
            }
            have_path[Y1[i]][X1[i]] = true;
            player_have_path[i].push_back({Y1[i], X1[i]});
        }
        int res = 0;
        if(!in_enemy()){
            res=max_solve();
        }else{
            res=solve();
        }
        cerr << "前段階終了" << endl;
        cout << output[res] << endl;
    }
}