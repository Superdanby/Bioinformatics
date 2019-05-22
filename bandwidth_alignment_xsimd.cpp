#include <omp.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>
#include "xsimd/xsimd.hpp"

using namespace std;
using vector_int = std::vector<int, xsimd::aligned_allocator<int, XSIMD_DEFAULT_ALIGNMENT>>;
namespace xs = xsimd;

constexpr int MAXN = 1e7;
constexpr int BANDWIDTH = 1e6;
constexpr int ninf = -0x7f7f7f7f;
// static char A[MAXN], B[MAXN];
#define DP_TYPE int
constexpr int match = 8, miss = -5, shift = -3;
xs::simd_type<int> simd_match(8), simd_miss(-5), simd_shift(-3);
const int nthreads = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;

int bio_match(string &a, string &b) {
    if (a.size() > b.size()) swap(a, b);
    reverse(a.begin(), a.end());
    vector_int A(a.begin(), a.end()), B(b.begin(), b.end());
    vector<vector_int> dp(3, vector_int(MAXN));
    // omp_set_num_threads(nthreads);
    int last_l = 0, last_r = 0;
    dp[0][0] = 0;
    dp[1][1] = dp[1][0] = shift;
    int reduce = 0;
    for (int i = 0; i < A.size() + B.size() - 1; i++) {
        int l = max(0, i - min(int(A.size()), BANDWIDTH) + 1), r = min(i, int(B.size()) - 1);
        int rr = r;
        if (i >= BANDWIDTH) {
            reduce += (i % 2 == 1);
            l -= reduce, r -= reduce;
            // 1 2 3 4 5 6 7 8
            // 1 1 2 2 3 3 4 4
            // 0 1 1 2 2 3 3 4
        }
        l = min(l, max(rr - BANDWIDTH, 0));
        if (i >= BANDWIDTH)
            r = l + BANDWIDTH;

        // int l = max(0, i - na + 1), r = min(i, nb - 1);
        // cout << "l: " << l << ", r: " << r << "\n";
        if (i < BANDWIDTH)
            dp[2][r + 2] = dp[2][0] = (i + 2) * shift;
        else {
            // l = min(l + 1, )
            dp[2][r + 2] = dp[2][l] = ninf;
        }
        std::size_t size = r - l + 1;
        std::size_t simd_size = xsimd::simd_type<int>::size;
        // std::cout << "size: " << simd_size << "\n";
        std::size_t vec_size = size - size % simd_size;
        for(std::size_t j = l; j < l + vec_size; j += simd_size)
        {
            int x = A.size() - (i - j) - 1, y = j;
            auto a_vec = xs::load_unaligned(&A[x]);
            auto b_vec = xs::load_unaligned(&B[y]);
            auto dp_0 = xs::load_unaligned(&dp[0][j]);
            auto dp_10 = xs::load_unaligned(&dp[1][j]);
            auto dp_11 = xs::load_unaligned(&dp[1][j + 1]);
            auto diag = xsimd::select(a_vec == b_vec, dp_0 + simd_match, dp_0 + simd_miss);
			auto up = xsimd::load_unaligned(&dp[1][j]);
			auto left = xsimd::load_unaligned(&dp[1][j + 1]);
			auto max_vec = xsimd::select(dp_10 > dp_11, dp_10 + simd_shift, dp_11 + simd_shift);
			auto max_dp = xsimd::select(diag > max_vec, diag, max_vec);

            max_dp.store_unaligned(&dp[2][j + 1]);
        }
        for(std::size_t j = l + vec_size; j <= r; ++j)
        {
            int x = A.size() - (i - j) - 1, y = j;
            dp[2][j + 1] = dp[0][j] + (A[x] == B[y] ? match : miss);
            dp[2][j + 1] = max(
                dp[2][j + 1],
                (dp[1][j] > dp[1][j + 1] ? dp[1][j] : dp[1][j + 1]) + shift);
        }
        copy(dp[1].begin() + last_l, dp[1].begin() + last_r + 3, dp[0].begin() + last_l);
        copy(dp[2].begin() + l, dp[2].begin() + r + 3, dp[1].begin() + l);
        // memcpy(dp[0] + last_l, dp[1] + last_l,
        //        sizeof(DP_TYPE) * (last_r - last_l + 3));
        // memcpy(dp[1] + l, dp[2] + l, sizeof(DP_TYPE) * (r - l + 3));
        last_l = l, last_r = r;
    }
    return dp[2][B.size()];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    string a, b;
    while (cin >> a >> b) {
        int score = bio_match(a, b);
        printf("%d\n", score);
    }
    return 0;
}
