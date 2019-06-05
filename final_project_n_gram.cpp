#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <cmath>
using namespace std;
const int mod7 = 1e9+7;
const int MAXN = 65536;
const int MATCH = 8;
const int MISMATCH = -5;
const int GAP = -3;
const float br = 0.03;
int Q_ = 7;  // Q_gram
int D_;   // 有更新就再往後找 D_ 個
int bw;

constexpr double ERR = 0.03;
constexpr int ninf = -0x7f7f7f7f;
using DP_TYPE = int;
constexpr int match = 8, miss = -5, shift = -3;

vector<int> b;
vector<string> S;
struct Hash {
  vector<int> h;
  Hash(const string &s) {
    int len = s.length();
    h.resize(len);
    h[0] = s[0];
    for(int i=1; i<len; ++i)
      h[i] = (1ll*h[i-1]*37 + s[i]) % mod7;
  }
  int q(int l, int r) {  // len can't be 0
    if(l==0) return h[r];
    int res = ((1ll) * h.at(r) - (1ll) * h.at(l-1) * b.at(r-l)) % mod7;
    return (res<0 ? res+mod7 : res);
  }
};
vector<pair<int, pair<int, int>>> sorted;
void build() {
  struct Pos { int n, i; };
  unordered_map<int, vector<Pos>> m;
  vector<vector<int>> cnt(S.size(), vector<int> (S.size()));
  b.resize(S.back().length());
  b[0] = 37;
  int len = S.back().length();
  for(int i=1; i<len; ++i)
    b[i] = (1ll*b[i-1]*37) % mod7;
  for(size_t i=0; i<S.size(); ++i) {
    Hash h(S[i]);
    for(size_t n=0; n+Q_<S[i].length(); ++n) {
      auto &v = m[h.q(n, n+Q_)];
      for(Pos p : v) if(abs(int(p.i-n)) < 1000) ++cnt[p.n][i];
    }
    for(size_t n=0; n+Q_<S[i].length(); ++n) {
      auto &v = m[h.q(n, n+Q_)];
      if(!v.empty() && v.back().n == i && abs(int(v.back().i-n)) < 900)
        continue;
      v.push_back({int(i), int(n)});
    }
  }
  for(size_t i=0; i<S.size(); ++i)
    for(size_t j=i+1; j<S.size(); ++j)
      sorted.push_back({cnt[i][j], {i, j}});
  sort(sorted.begin(), sorted.end());
}


int align(string &a, string &b) {
    if (a.length() > b.length()) swap(a, b);
    const int BANDWIDTH = a.length() * ERR;
    vector<vector<DP_TYPE>> dp(3, vector<DP_TYPE>(int((a.length() + b.length()) * 1.2), 0));
    int last_l = 0, last_r = 0;
    dp[0][0] = 0;
    dp[1][1] = dp[1][0] = shift;
    int reduce = 0;
    // cerr << a.substr(0, 9) << "\n";
    for (int i = 0; i < a.length() + b.length() - 1; i++) {
        int l = max(0, i - min(int(a.length()), BANDWIDTH) + 1), r = min(i, int(b.length()) - 1);
        int rr = r;
        if (i >= BANDWIDTH) {
            reduce += (i % 2 == 1);
            l -= reduce, r -= reduce;
        }
        l = min(l, max(rr - BANDWIDTH, 0));
        if (i < BANDWIDTH)
            dp[2][r + 2] = dp[2][0] = (i + 2) * shift;
        else {
            r = l + BANDWIDTH;
            // l = min(l + 1, )
            dp[2][r + 2] = dp[2][l] = ninf;
        }
        int j = l;
        auto it_2 = dp[2].begin() + j + 1, it_2end = dp[2].begin() + r + 2;
        auto it_1 = dp[1].begin() + j, it_11 = dp[1].begin() + j + 1, it_0 = dp[0].begin() + j;
        auto it_a = a.begin() + i - j, it_b = b.begin() + j;
        for (;it_2 != it_2end; ++it_2)
        {
            *it_2 = *it_0 + (*it_a == *it_b ? match : miss);
            auto it_shift = (*it_1 > *it_11 ? *it_1 : *it_11) + shift;
            *it_2 = (it_shift > *it_2 ? it_shift : *it_2);
            ++j, ++it_1, ++it_11, ++it_0, --it_a, ++it_b;
        }
        dp[0].swap(dp[1]);
        dp[1].swap(dp[2]);
        // copy(dp[1].begin() + last_l, dp[1].begin() + last_r + 3, dp[0].begin() + last_l);
        // copy(dp[2].begin() + l, dp[2].begin() + r + 3, dp[1].begin() + l);
        last_l = l, last_r = r;
    }
    // return dp[2][b.length()];
    return dp[1][b.length()];
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  string str;
  while(cin >> str) S.push_back(str);
  bw = S.back().length() * br;
  D_ = sqrt(S.size()) + 5;
  //printf("get %d strings. bandwith=%d. step=%d.\nbuilding...\n", S.size(), bw, D_);
  build();
  //printf("build done. running...\n");

  int mn = 1e9, mx = -mn, t;
  int N = sorted.size(), i,k,n;
  for(k=i=0; i<D_ && i+k<N; ++i) {
    t = align(S[sorted[i+k].second.first], S[sorted[i+k].second.second]);
    //printf("%d : %d   gram:%d\n", i+k, t, sorted[i+k].first);
    if(t <= mn) k+=i, i=0, mn = t;
    if(t > mx) mx = t;
  }
  n = i+k;
  for(k=i=0; i<D_ && N-1-i-k>=n; ++i) {
    t = align(S[sorted[N-1-i-k].second.first], S[sorted[N-1-i-k].second.second]);
    //printf("%d : %d  gram:%d\n", N-1-i-k, t, sorted[N-1-i-k].first);
    if(t < mn) mn = t;
    if(t >= mx) k+=i, i=0, mx = t;
  }

  cout << "max: " << mx << " min: " << mn << endl;
}
