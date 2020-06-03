/*
    Author:zeke

    pass System Test!
    GET AC!!
*/
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
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
typedef pair<ll, ll> P;
typedef vector<ll> V;
typedef vector<V> VV;
typedef vector<P> VP;
ll mod = 1e9 + 7;
ll MOD = 1e9 + 7;
ll INF = 1e18;
// cout << "Case #" << index << " :IMPOSSIBLE";
void input(vector<int>& vec) {
    cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
            "||||||||||"
         << endl;
    cout << "ただいまのゲーム状態" << endl;
    for (int i = 0; i < vec.size(); i++) {
        cout << i + 1 << "番目"
             << " " << vec[i] << endl;
    }
    while (1) {
        cout << "山の番号といくつ減らすかを記入してください" << endl;
        int index, num;
        cin >> index >> num;
        if (vec[index - 1] >= num) {
            cout << "あなたは山" << index << "から" << num << "減らしました"
                 << endl;
            vec[index - 1] -= num;
            break;
        } else {
            cout << "正しく入力してください" << endl;
        }
    }
    cout << "入力成功" << endl;
    cout << "ただいまのゲーム状態" << endl;
    for (int i = 0; i < vec.size(); i++) {
        cout << i + 1 << "番目"
             << " " << vec[i] << endl;
    }
}
void output(int index, int k) {
    cout << "山" << index + 1 << "から" << k << "取り除く" << endl;
}
vector<vector<int>> status;    //ノードの状態
vector<long double> evaluate;  //ノードの評価値
vector<int> history;           //ノードの探索回数
vector<vector<int>> graph;     //モンテカルロ木の隣接リスト
vector<bool> child_exist;      //すでに子ノードを展開済みか？
vector<int> parent;            //親のindex
vector<bool> playout;          // playoutの状態か？
const int expasion_limit = 100;  //ノード展開のための閾値
const int search_num = 10000;
int prev_state = 0;
int total_try_num=0;
const long double C_p =1;  //コスト関数のハイパーパラメーター
long double UCB1(long double w) {
    //cout<<t<<" "<<w<<endl;
    return C_p * (log(2.00 *(long double) (total_try_num + 1)) / (w + 1.00));
    // wは指す予定の行動が過去に何回されたか
}
int Selection(int s,int start) {
    // cout<<"今から"<<s<<"を頂点としたSelectionをしますわ"<<endl;
    if (playout[s]) {
        return -1;
    }
    if (child_exist[s]) {
        priority_queue<pair<long double, int>> q;
        queue<int> state_list;
        for (auto i : graph[s]) {
            state_list.push(i);
        }
        while (!state_list.empty()) {
            int now = state_list.front();
            state_list.pop();
            if (playout[now] || (history[now] < expasion_limit)) {
                if(playout[now]&&history[now]!=0){
                    q.push({evaluate[now] + UCB1(history[now])-INF, now});
                }else{
                    q.push({evaluate[now] + UCB1(history[now]), now});
                }
            } else {
                for (auto child : graph[now]) {
                    state_list.push(child);
                }
            }
        }
        return q.top().second;
    } else {
        child_exist[s] = true;
        //子ノードの拡張を行う
        for (int i = 0; i < status[s].size(); i++) {
            for (int j = 1; j <= status[s][i]; j++) {
                vector<int> temp = status[s];
                temp[i] -= j;
                bool zero = true;
                for (auto k : temp) {
                    if (k != 0) {
                        zero = false;
                        break;
                    }
                }
                status.push_back(temp);
                evaluate.push_back(0.00);
                history.push_back(0);
                graph[s].push_back(status.size() - 1);
                graph.push_back({});
                child_exist.push_back(false);
                parent.push_back(s);
                if (zero) {
                    playout.push_back(true);
                } else {
                    playout.push_back(false);
                }
            }
        }
        // cout<<"子ノードの拡張をして"<<graph[s][0]<<"へ潜りましたわ！"<<endl;
        return graph[s][0];
    }
}
pair<int, int> MonteCarloTreeSearch(vector<int> vec) {
    cout << "モンテカルロ探索を開始しますわ" << endl;
    int start;
    if (status[prev_state] == vec) {
        start = prev_state;
    }
    for (int i = 0; i < graph[prev_state].size(); i++) {
        if (status[graph[prev_state][i]] == vec) {
            start = graph[prev_state][i];
        }
    }
    cout << "スタートノードは" << start << endl;
    for (int search = 0; search < search_num; search++) {
        total_try_num++;
        int now = start;
        // playoutまで探索するのです！
        while (1) {
            int child = Selection(now,start);
            if (child == -1) {
                break;
            }
            now = child;
        }
        //葉ノードからさかのぼりますわ！
        int temp = now;
        bool init = true;
        int path = 0;
        //経路長の探索
        while (temp != start) {
            path++;
            temp = parent[temp];
        }
        long double score;
        if(history[now]!=0){
            score=0;
        }else if (path % 2 == 0) {
            score = -1;
        } else {
            score = 1;
        }
        //経路ノードの更新
        while (now != start) {
            if (init) {
                evaluate[now] = score;
                history[now]++;
                init=false;
            } else {
                long double evaluate_sum =(long double)(history[now]) * evaluate[now];
                evaluate_sum += score;
                history[now]++;
                if(score!=0){
                    evaluate[now] = (long double)(evaluate_sum) / (long double)(history[now]);
                }
            }
            now = parent[now];
        }
        history[start]++;
    }
    int max_reg = 0;
    int res;
    for (int i = 0; i < graph[start].size(); i++) {
        cout << "状態" << graph[start][i] << "の探索回数" << history[graph[start][i]]<<"状態評価値"<<evaluate[graph[start][i]]<< endl;
        if (chmax(max_reg, history[graph[start][i]])) {
            res = graph[start][i];
        }
    }
    prev_state = res;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] != status[res][i]) {
            return {i, vec[i] - status[res][i]};
        }
    }
}
void initial(vector<int>& vec) {
    status.push_back(vec);
    evaluate.push_back(0.00);
    history.push_back(0);
    graph.push_back({});
    child_exist.push_back(false);
    parent.push_back(-1);
    playout.push_back(false);
}
int main() {
    int n;
    cin >> n;
    vector<int> vec(n);
    for (int i = 0; i < n; i++) {
        cin >> vec[i];
    }
    initial(vec);
    cout << "先手:1,後手:2" << endl;
    int type;
    cin >> type;
    if (type == 1) {
        input(vec);
        initial(vec);
        graph[0].push_back(1);
    }
    while (1) {
        bool f2 = true;
        for (auto i : vec) {
            if (i != 0) {
                f2 = false;
            }
        }
        if (f2) {
            cout << "私の負けです" << endl;
            return 0;
        }
        pair<int, int> res = MonteCarloTreeSearch(vec);
        output(res.F, res.S);
        vec[res.F] -= res.S;
        int XOR_sum=0;
        for(auto i:vec){
            XOR_sum^=i;
        }
        cout<<"排他的論理和"<<XOR_sum<<endl;
        bool f = true;
        for (auto i : vec) {
            if (i != 0) {
                f = false;
            }
        }
        if (f) {
            cout << "俺の勝ち！何で負けたか、明日まで考えといてください。そした"
                    "ら何かが見えてくるはずです。ほな、いただきます。"
                 << endl;
            return 0;
        }
        input(vec);
    }
}