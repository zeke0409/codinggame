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
void input(V &vec){
    cout<<"ただいまのゲーム状態"<<endl;
    rep(i,vec.size()){
        cout<<i+1<<"番目"<<" "<<vec[i]<<endl;
    }
    while(1){
        cout<<"山の番号といくつ減らすかを記入してください"<<endl;
        ll index,num;
        cin>>index>>num;
        if(vec[index-1]>=num){
            cout<<"あなたは山"<<index<<"から"<<num<<"減らしました"<<endl;
            vec[index-1]-=num;
            break;
        }else{
            cout<<"正しく入力してください"<<endl;
        }
    }
    cout<<"入力成功"<<endl;
    cout<<"ただいまのゲーム状態"<<endl;
    rep(i,vec.size()){
        cout<<i+1<<"番目"<<" "<<vec[i]<<endl;
    }
}
void output(ll index,ll k){
    cout<<"山"<<index+1<<"から"<<k<<"取り除く"<<endl;
}
int MSB8bit(int v) {
    v |= (v >> 1);
    v |= (v >> 2);
    v |= (v >> 4);
    return __builtin_popcount(v);
}
int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(10);
    ll n;
    cin>>n;
    V vec(n);
    rep(i,n){
        cin>>vec[i];
    }
    cout<<"先手:1,後手:2"<<endl;
    ll type;
    cin>>type;
    if(type==1){
        input(vec);
    }
    while(1){
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
        ll XOR_sum=0;
        ll sum=0;
        for(auto i:vec){
            XOR_sum^=i;
            sum+=i;
        }
        if(XOR_sum==0){
            rep(i, vec.size()) {
                if (vec[i] != 0) {
                    output(i, 1);
                    vec[i]--;
                    break;
                }
            }
        }else{
            rep(i,vec.size()){
                if(MSB8bit(XOR_sum)==MSB8bit(vec[i])){
                    int res=vec[i]^XOR_sum;
                    output(i,vec[i]-res);
                    vec[i]=res;
                    break;
                }
            }
            bool f=true;
            for(auto i:vec){
                if(i!=0){
                    f=false;
                }
            }
            if(f){
                cout<<"俺の勝ち！何で負けたか、明日まで考えといてください。そしたら何かが見えてくるはずです。ほな、いただきます。"<<endl;
                return 0;
            }
        }
        input(vec);
    }
}
