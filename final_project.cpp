#include <omp.h>
#include <cassert>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

using namespace std;

constexpr double ERR = 0.003;
constexpr int ninf = -0x7f7f7f7f;
using DP_TYPE = int;
constexpr int match = 8, miss = -5, shift = -3;
const int nthreads = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;
typedef long long ll;
// memory compress and move

int bio_match_cpp(string &a, string &b) {
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

int bio_match_c(const char *A, int na, const char *B, int nb) {
    if (na > nb) swap(A, B), swap(na, nb);
    const int BANDWIDTH = na * ERR;
    DP_TYPE dp[3][int((na + nb) * 1.5)];
    // DP_TYPE * dp[3];
    // DP_TYPE ** dp = (DP_TYPE **)malloc(3 * sizeof(DP_TYPE *));
    // for (int i = 0; i < 3; ++i)
    //     dp[i] = (DP_TYPE *)malloc(int((na + nb) * 1.5) * sizeof(DP_TYPE));

    int last_l = 0, last_r = 0;
    dp[0][0] = 0;
    dp[1][1] = dp[1][0] = shift;
    int reduce = 0;
    for (int i = 0; i < na + nb - 1; i++) {
        int l = max(0, i - min(na, BANDWIDTH) + 1), r = min(i, nb - 1);
        int rr = r;
        if (i >= BANDWIDTH) {
            reduce += (i % 2 == 1);
            l -= reduce, r -= reduce;
            // 1 2 3 4 5 6 7 8
            // 1 1 2 2 3 3 4 4
            // 0 1 1 2 2 3 3 4
        }
        l = min(l, max(rr - BANDWIDTH, 0));

        // int l = max(0, i - na + 1), r = min(i, nb - 1);
        // cout << "l: " << l << ", r: " << r << "\n";
        if (i < BANDWIDTH)
            dp[2][r + 2] = dp[2][0] = (i + 2) * shift;
        else {
            r = l + BANDWIDTH;
            // l = min(l + 1, )
            dp[2][r + 2] = dp[2][l] = ninf;
        }
        for (int j = l; j <= r; j++) {
            int x = i - j, y = j;
            dp[2][j + 1] = dp[0][j] + (A[x] == B[y] ? match : miss);
            dp[2][j + 1] = max(
                dp[2][j + 1],
                (dp[1][j] > dp[1][j + 1] ? dp[1][j] : dp[1][j + 1]) + shift);
        }
        // for (int z = 0; z < 10; z++)
        //     cout << dp[2][z] << " ";
        // cout << "\n";
        memcpy(dp[0] + last_l, dp[1] + last_l,
               sizeof(DP_TYPE) * (last_r - last_l + 3));
        memcpy(dp[1] + l, dp[2] + l, sizeof(DP_TYPE) * (r - l + 3));
        last_l = l, last_r = r;
    }
    return dp[2][nb];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    omp_set_num_threads(nthreads);
    vector<string> pool;
    string A;
    while (cin >> A) {
        pool.push_back(string(A));
    }
    vector<ll> result(pool.size() * (pool.size() - 1) / 2);
    size_t idx = 0;
    // ll ans_max = 0, ans_min = 1e18;
#pragma omp parallel for
    for (size_t i = 0; i < pool.size(); ++i)
#pragma omp parallel for
        for (size_t j = i + 1; j < pool.size(); ++j)
        {
            // result[i * pool.size() + j - i - i * (i + 1) / 2 - 1 = bio_match_c(pool[i].c_str(), pool[i].length(), pool[j].c_str(), pool[j].length());
            result[i * pool.size() + j - i - i * (i + 1) / 2 - 1] = bio_match_cpp(pool[i], pool[j]);
        }
    ll ans_max = 0, ans_min = 1e18;
    idx = 0;
    for (size_t i = 0; i < pool.size(); ++i)
        for (size_t j = i + 1; j < pool.size(); ++j)
        {
            ans_min = min(ans_min, ll(result[i * pool.size() + j - i - i * (i + 1) / 2 - 1]));
            ans_max = max(ans_max, ll(result[i * pool.size() + j - i - i * (i + 1) / 2 - 1]));
            cout << i << " " << j << ": " << result[i * pool.size() + j - i - i * (i + 1) / 2 - 1] << "\n";
        }
    cout << "MAX: " << ans_max << "\n"
        << "min: " << ans_min << "\n";
    return 0;
}
