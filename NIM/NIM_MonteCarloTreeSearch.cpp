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
    cout << "ただいまのゲーム状態" << endl;
    for(int i=0;i<vec.size();i++) {
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
    for(int i=0;i<vec.size();i++) {
        cout << i + 1 << "番目"
             << " " << vec[i] << endl;
    }
}
void output(int index, int k) {
    cout << "山" << index + 1 << "から" << k << "取り除く" << endl;
}
vector<vector<int>> status;//ノードの状態
vector<long double> evaluate;//ノードの評価値
vector<int> history;//ノードの探索回数
vector<vector<int>> graph;//モンテカルロ木の隣接リスト
vector<bool> child_exist;//すでに子ノードを展開済みか？
vector<int> parent;//親のindex
vector<bool> playout;//playoutの状態か？
const int expasion_limit=3;//ノード展開のための閾値
const int search_num=1000;
int prev_state=0;
const long double C_p=0.5;//コスト関数のハイパーパラメーター
long double UCB1(long double w,long double t){
    return C_p*(log(2.00*t)/(w+1.00));
    // tは総試行回数
    // wは指す予定の行動が過去に何回されたか
}
int Selection(int s){
    if(playout[s]){
        return -1;
    }
    if(child_exist[s]){
        priority_queue<pair<long double,int>> q;
        queue<int> state_list;
        for(auto i:graph[s]){
            state_list.push(i);
        }
        while(!state_list.empty()){
            int now=state_list.front();
            state_list.pop();
            if(playout[now]||history[now]<expasion_limit){
                q.push({evaluate[now]+UCB1(history[s],history[now]),now});
            }else{
                for(auto child:graph[now]){
                    state_list.push(child);
                }
            }
        }
        return q.top().second;
    }else{
        child_exist[s]=true;
        //子ノードの拡張を行う
        for(int i=0;i<status[s].size();i++){
            for(int j=1;j<status[s][i];j++){
                vector<int> temp = status[s];
                temp[i]-=j;
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
                graph[s].push_back(status.size()-1);
                graph.push_back({});
                child_exist.push_back(false);
                parent.push_back(s);
                if(zero){
                    playout.push_back(true);
                }else{
                    playout.push_back(false);
                }
            }
        }
        return graph[s][0];
    }
}
pair<int,int> MonteCarloTreeSearch(vector<int> vec){
    int start;
    if(status[prev_state]==vec){
        start=prev_state;
    }
    for(int i=0;i<graph[prev_state].size();i++){
        if(status[graph[prev_state][i]]==vec){
            start=graph[prev_state][i];
        }
    }
    for(int search=0;search<search_num;search++){
        int now=start;
        //playoutまで探索するのです！
        
        while(1){
            int child = Selection(now);
            if(child==-1){
                break;
            }
        }
        //葉ノードからさかのぼりますわ！
        int temp=now;
        bool init=true;
        int path=0;
        //経路長の探索
        //cout<<"経路"<<endl;
        while(temp!=start){
            //cout<<temp<<" ";
            path++;
            temp=parent[temp];
        }
        //cout<<endl;
        long double score;
        if(path%2==0){
            score=-1;
        }else{
            score=1;
        }
        //経路ノードの更新
        while(now!=start){
            if(init){
                evaluate[now]=score;
                history[now]++;
            }else{
                long double evaluate_sum=(long double)(history[now])*evaluate[now];
                evaluate_sum+=score;
                history[now]++;
                evaluate[now] = evaluate_sum / (long double)(history[now]);
            }
            now=parent[now];
        }
    }
    int max_reg=0;
    int res;
    for(int i=0;i<graph[start].size();i++){
        if(chmax(max_reg,history[i])){
            res=max_reg;
        }
    }
    for(int i=0;i<vec.size();i++){
        if(vec[i]!=status[res][i]){
            return {i,vec[i]-status[res][i]};
        }
    }
}
void initial(vector<int> &vec){
    status.push_back(vec);
    evaluate.push_back(0.00);
    history.push_back(0);
    graph.push_back({});
    child_exist.push_back(false);
    parent.push_back(-1);
    playout.push_back(false);
}
int main(){
    int n;
    cin >> n;
    vector<int> vec(n);
    for(int i=0;i<n;i++) { 
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
    
}