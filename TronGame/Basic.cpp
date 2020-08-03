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
vector<vector<bool>> have_path(20,
                               vector<bool>(30, false));  // record players went
vector<vector<pair<int, int>>> player_have_path(4);       // record players went
vector<bool> death(4);                                    // whether die or not
string output[] = {"UP", "DOWN", "LEFT", "RIGHT"};
int dx[] = {0, 0, -1, 1};
int dy[] = {-1, 1, 0, 0};
int N;
int P;
int playerN;  // players who are contanted
int my_num;
vector<int> final_res;
set<int> in_enemy_set;  // contacting enemy list
vector<int> X0(4), Y0(4), X1(4), Y1(4);
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
vector<vector<int>> BFS_distance(int y, int x) {  // map from (y,x) O(H*W)
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
// number of coordinates that can go from (y,x)
int BFS_num(int y, int x, vector<vector<bool>> have, bool k = false) {
    have[y][x] = true;
    int res = 0;
    for (int way = 0; way < 4; way++) {
        if (!can_move(y + dy[way], x + dx[way], have)) {
            continue;
        }
        vector<vector<bool>> have2 = have;
        queue<int> q;
        q.push((y + dy[way]) * 30 + (x + dx[way]));
        int cnt = 0;
        while (!q.empty()) {
            cnt++;
            int nowy = q.front() / 30;
            int nowx = q.front() % 30;
            have2[nowy][nowx] = true;
            q.pop();
            for (int i = 0; i < 4; i++) {
                int nowy2 = nowy + dy[i];
                int nowx2 = nowx + dx[i];
                if (nowy2 >= 0 && nowy2 < 20 && nowx2 >= 0 && nowx2 < 30) {
                    if (have2[nowy2][nowx2]) {
                        continue;
                    }
                    have2[nowy2][nowx2] = true;
                    q.push(nowy2 * 30 + nowx2);
                }
            }
        }
        chmax(res, cnt);
    }
    return res;
}

set<int> in_enemy(int y, int x, vector<vector<bool>> temp) {
    queue<vector<int>> q;
    set<int> st;
    q.push({y, x});
    while (!q.empty()) {
        int nowy = q.front()[0];
        int nowx = q.front()[1];
        q.pop();
        for (int way = 0; way < 4; way++) {
            for (int player = 0; player < N; player++) {
                if (P == player || death[player]) {
                    continue;
                }
                if (nowy + dy[way] == Y1[player] &&
                    nowx + dx[way] == X1[player]) {
                    st.insert(player);
                }
            }
            if (can_move(nowy + dy[way], nowx + dx[way], temp)) {
                temp[nowy + dy[way]][nowx + dx[way]] = true;
                q.push({nowy + dy[way], nowx + dx[way]});
            }
        }
    }
    return st;
}
vector<vector<bool>> alphabeta_have(20, vector<bool>(30));
vector<pair<int, int>> alphabeta_pos(3);
bool alphabeta_in_enemy(int y, int x, vector<vector<bool>> temp) {
    queue<vector<int>> q;
    q.push({y, x});
    while (!q.empty()) {
        int nowy = q.front()[0];
        int nowx = q.front()[1];
        q.pop();
        for (int way = 0; way < 4; way++) {
            for (int player = 0; player < playerN; player++) {
                if (player == my_num) {
                    continue;
                }
                if (nowy + dy[way] == alphabeta_pos[player].F &&
                    nowx + dx[way] == alphabeta_pos[player].S) {
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
int alphabeta(int limit, int depth, bool type) {
    int res;
    int nowy = alphabeta_pos[(depth + my_num) % playerN].F;
    int nowx = alphabeta_pos[(depth + my_num) % playerN].S;
    if (depth == 3) {
        res = 0;
        vector<pair<int, int>> vec;
        for (int player = 0; player < playerN; player++) {
            int k = BFS_num(alphabeta_pos[player].F, alphabeta_pos[player].S,
                            alphabeta_have);
            vec.push_back({k, player});
            if (player == my_num) {
                res += k;
            } else {
                res -= k;
            }
        }
        sort(all(vec), greater<pair<int, int>>());
        if (!alphabeta_in_enemy(alphabeta_pos[my_num].F,
                                alphabeta_pos[my_num].S, alphabeta_have)) {
            if (vec[0].F != vec[1].F) {
                if (vec[0].S == my_num) {
                    res += INF / 2;
                } else if (vec[1].F != vec[vec.size() - 1].F) {
                    res -= INF / 2;
                }
            }
            if (vec[0].F == vec[1].F && vec[0].S != my_num &&
                vec[1].S != my_num) {
                if (vec[0].F < vec[2].F * 2) {
                    res += INF / 4;
                }
            }
        }
        return res;
    }
    if (depth % playerN == 0) {
        int reg = 0;
        int zeroreg = -INF - 1;
        int regres = INF + 100;
        res = -INF;
        for (int j = 0; j < 4; j++) {
            if (can_move(nowy + dy[j], nowx + dx[j], alphabeta_have)) {
                alphabeta_pos[(depth + my_num) % playerN].F = nowy + dy[j];
                alphabeta_pos[(depth + my_num) % playerN].S = nowx + dx[j];
                alphabeta_have[nowy + dy[j]][nowx + dx[j]] = true;
                int k = alphabeta(regres, depth + 1, 0);
                alphabeta_have[nowy + dy[j]][nowx + dx[j]] = false;
                alphabeta_pos[(depth + my_num) % playerN].F = nowy;
                alphabeta_pos[(depth + my_num) % playerN].S = nowx;
                if (k == 2 * INF) {
                    if (depth != 0) {
                        continue;
                    }
                }
                if (k <= limit) {
                    if (depth != 0) {
                        return 2 * INF;
                    }
                }
                if (depth == 0) {
                    cerr << output[j] << "方向" << k
                         << "|||||||||||||||||||||||||||||||" << endl;
                    if (k == 2 * INF) {
                        continue;
                    }
                    if (abs(zeroreg - k) == 0) {
                        final_res.push_back(j);
                    } else if (chmax(zeroreg, k)) {
                        final_res = {j};
                    }
                } else {
                    chmax(regres, k);
                    chmax(res, k);
                }
            }
        }
    } else if ((depth + 1) % playerN != 0) {
        res = INF;
        int reg = -INF - 100;
        for (int j = 0; j < 4; j++) {
            if (can_move(nowy + dy[j], nowx + dx[j], alphabeta_have)) {
                alphabeta_pos[(depth + my_num) % playerN].F = nowy + dy[j];
                alphabeta_pos[(depth + my_num) % playerN].S = nowx + dx[j];
                alphabeta_have[nowy + dy[j]][nowx + dx[j]] = true;
                int k = alphabeta(reg, depth + 1, 1);
                alphabeta_have[nowy + dy[j]][nowx + dx[j]] = false;
                alphabeta_pos[(depth + my_num) % playerN].F = nowy;
                alphabeta_pos[(depth + my_num) % playerN].S = nowx;
                if (k == 2 * INF) {
                    continue;
                }
                if (type == 0 && limit <= k) {
                    if (depth == 1 && limit == k) {
                        return k;
                    }
                    return 2 * INF;
                }
                if (type == 1 && limit >= k) {
                    return 2 * INF;
                }
                chmin(res, k);
            }
        }
    } else {
        int regres = -INF - 100;
        res = INF;
        for (int j = 0; j < 4; j++) {
            if (can_move(nowy + dy[j], nowx + dx[j], alphabeta_have)) {
                alphabeta_pos[(depth + my_num) % playerN].F = nowy + dy[j];
                alphabeta_pos[(depth + my_num) % playerN].S = nowx + dx[j];
                alphabeta_have[nowy + dy[j]][nowx + dx[j]] = true;
                int k = alphabeta(regres, depth + 1, 1);
                alphabeta_have[nowy + dy[j]][nowx + dx[j]] = false;
                alphabeta_pos[(depth + my_num) % playerN].F = nowy;
                alphabeta_pos[(depth + my_num) % playerN].S = nowx;
                if (k == 2 * INF) {
                    continue;
                }
                if (type == 1 && limit >= k) {
                    return 2 * INF;
                }
                if (type == 0 && limit <= k) {
                    if (depth == 1 && limit == k) {
                        return k;
                    }
                    return 2 * INF;
                }
                chmin(regres, k);
                chmin(res, k);
            }
        }
    }
    return res;
}
int solve() {
    alphabeta_have = have_path;
    return alphabeta(INF, 0, 0);
}
int max_solve() {  // if not contacting enemies
    int reg = 0;
    vector<int> res;
    for (int i = 0; i < 4; i++) {
        int nowy = Y1[P] + dy[i];
        int nowx = X1[P] + dx[i];
        if (can_move(nowy, nowx, have_path)) {
            int possible = BFS_num(nowy, nowx, have_path, false);
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
        cerr << output[i] << " " << i << " " << tempreg << " " << secondreg
             << endl;
        if (secondreg == tempreg) {
            res2.push_back(i);
        } else if (secondreg < tempreg) {
            res2 = {i};
            secondreg = tempreg;
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
void input() {  // input process
    cin >> N >> P;
    for (int i = 0; i < N; i++) {
        cin >> X0[i] >> Y0[i] >> X1[i] >> Y1[i];
        if (X1[i] == -1) {
            if (!death[i]) {  // if death player is found in this turn
                death_process(i);
                death[i] = true;
            }
            continue;
        }
        // Record of the places player went.
        have_path[Y1[i]][X1[i]] = true;
        have_path[Y0[i]][X0[i]] = true;
        player_have_path[i].push_back({Y1[i], X1[i]});
    }
    in_enemy_set = in_enemy(Y1[P], X1[P], have_path);
    playerN = 0;  // substantial number of competitors
    alphabeta_pos = {};
    for (int i = 0; i < N; i++) {
        bool f = false;
        for (auto j = in_enemy_set.begin(); j != in_enemy_set.end(); j++) {
            if (i == *j) {
                f = true;
                break;
            }
        }
        if (f || i == P) {
            playerN++;
            alphabeta_pos.push_back({Y1[i], X1[i]});
            if (i == P) {
                my_num = alphabeta_pos.size() - 1;
            }
        }
    }
}
int Boronoi(){
    vector<vector<int>> FINAL_distance = BFS_distance(Y1[P], X1[P]);
    int near = 0;
    int near_reg = INF;
    for (int i = 0; i < N; i++) {
        if (death[i] || i == P) {
            continue;
        }
        if (chmin(near_reg, FINAL_distance[Y1[i]][X1[i]])) {
            near = i;
        }
    }
    priority_queue<pair<int, int>> q;
    for (auto way : final_res) {
        int SCORE = 0;
        vector<vector<int>> Voronoi(20, vector<int>(30, -1));
        vector<vector<int>> Voronoi_reg(20, vector<int>(30, INF - 1));
        for (int i = 0; i < N; i++) {
            if (death[i]) {
                continue;
            }
            if (i == P) {
                Y1[i] += dy[way];
                X1[i] += dx[way];
                have_path[Y1[i]][X1[i]] = true;
                if (Y1[i] == 19 || Y1[i] == 0 || X1[i] == 29 || X1[i] == 0) {
                    SCORE -= 10;
                }
            }
            vector<vector<int>> BFS_dis = BFS_distance(Y1[i], X1[i]);
            if (i == P) {
                have_path[Y1[i]][X1[i]] = false;
                Y1[i] -= dy[way];
                X1[i] -= dx[way];
            }
            for (int h = 0; h < 20; h++) {
                for (int w = 0; w < 30; w++) {
                    if (chmin(Voronoi_reg[h][w], BFS_dis[h][w])) {
                        Voronoi[h][w] = i;
                    }
                }
            }
        }
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 30; j++) {
                if (Voronoi[i][j] == -1) {
                    // cerr << "K";
                } else {
                    if (Voronoi[i][j] == P) {
                        SCORE++;
                    } else if (Voronoi[i][j] == near) {
                        SCORE--;
                    }
                }
            }
        }
        bool F1 = true;

        for (int i = 0; i < N; i++) {
            if (i == P || death[i]) {
                continue;
            }
            if (FINAL_distance[Y1[i]][X1[i]] < 10) {
                F1 = false;
            }
        }
        if (F1) {
            cerr << "遠い" << endl;
            for (int k = 0; k < 4; k++) {
                int NOWY = Y1[P] + dy[way] + dy[k];
                int NOWX = X1[P] + dx[way] + dx[k];
                if (!can_move(NOWY, NOWX, have_path)) {
                    SCORE += 30;
                }
            }
        }

        cerr << "ボロノイ" << output[way] << " " << SCORE << endl;
        q.push({SCORE, way});
    }
    return q.top().S;
}
int main() {
    while (1) {
        input();  // input process
        cerr << "INPUT" << endl;
        int res = 0;
        if (in_enemy_set.empty()) {  // No contacting enemy
            res = max_solve();
            cout << output[res] << endl;
        } else {
            // alphabeta search
            res = solve();
            for (auto i : final_res) {
                cerr << "候補" << output[i] << endl;
            }
            res=Boronoi();
            cout<<output[res]<<endl;
        }
    }
}