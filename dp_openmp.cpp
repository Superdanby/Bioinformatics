#include <omp.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>

using namespace std;

const int MAXN = 1e7;
static char A[MAXN], B[MAXN];
#define DP_TYPE int
const int match = 8, miss = -5, shift = -3;
const int nthreads = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;

int bio_match(const char *A, int na, const char *B, int nb) {
    if (na > nb) swap(A, B), swap(na, nb);
    static DP_TYPE dp[3][MAXN];
    omp_set_num_threads(nthreads);
    int last_l = 0, last_r = 0;
    dp[0][0] = 0;
    dp[1][1] = dp[1][0] = shift;
    for (int i = 0; i < na + nb - 1; i++) {
        int l = max(0, i - na + 1), r = min(i, nb - 1);
        dp[2][r + 2] = dp[2][0] = (i + 2) * shift;
#pragma omp parallel for schedule(static) firstprivate(na, A, B)
        for (int j = l; j <= r; j++) {
            int x = i - j, y = j;
            dp[2][j + 1] = dp[0][j] + (A[x] == B[y] ? match : miss);
            dp[2][j + 1] = max(
                dp[2][j + 1],
                (dp[1][j] > dp[1][j + 1] ? dp[1][j] : dp[1][j + 1]) + shift);
        }
        memcpy(dp[0] + last_l, dp[1] + last_l,
               sizeof(DP_TYPE) * (last_r - last_l + 3));
        memcpy(dp[1] + l, dp[2] + l, sizeof(DP_TYPE) * (r - l + 3));
        last_l = l, last_r = r;
    }
    return dp[2][nb];
}

int main() {
    while (scanf("%s %s", A, B) == 2) {
        string a = A, b = B;
        int score = bio_match(a.c_str(), strlen(A), b.c_str(), strlen(B));
        printf("%d\n", score);
    }
    return 0;
}
