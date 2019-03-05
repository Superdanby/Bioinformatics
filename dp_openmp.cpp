#include <omp.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
const int MAXN = 10;
static char A[MAXN], B[MAXN];
#define DP_TYPE int
int lcs_len(const char *A, int na, const char *B, int nb, int dpf[]) {
    const int match = 8, mismatch = -5, shift = -3;
    if (na > nb) swap(A, B), swap(na, nb);
    static DP_TYPE dp[3][MAXN << 1];
    for (int i = 0; i < 3; i++)
        memset(dp[i], 0, sizeof(DP_TYPE) * (nb + na + 2));
    memset(dpf, 0, sizeof(DP_TYPE) * (nb + 1));
    omp_set_num_threads(8);
    int last_l = 0, last_r = 0;
    dp[1][1] = dp[1][0] = shift;
    for (int i = 0; i < na + nb - 1; i++) {
        // 0 [na - 1] 0 [nb] nb, 0 [nb - 1] nb - 1 [na] nb - 1
        int l = max(0, i - na + 1), r = min(i, nb - 1);
        dp[2][r + 2] = dp[2][0] = (i + 2) * shift;
#pragma omp parallel for schedule(static) firstprivate(na, A, B)
        for (int j = l; j <= r; j++) {
            int x = i - j, y = j;
            // printf("x = %d, y = %d\n", x, y);
            dp[2][j + 1] = dp[0][j] + (A[x] == B[y] ? match : mismatch);
            dp[2][j + 1] = max(
                dp[2][j + 1],
                (dp[1][j] > dp[1][j + 1] ? dp[1][j] : dp[1][j + 1]) + shift);
        }
        if (i - l == na - 1) dpf[l + 1] = dp[2][l + 1];
        // for (auto w : dp[2]) printf("%4d ", w);
        // printf("\n");
        // for (int z = 0; z < MAXN; z++) printf("%4d ", dpf[z]);
        // printf("\n");
        memcpy(dp[0] + last_l, dp[1] + last_l,
               sizeof(DP_TYPE) * (last_r - last_l + 3));
        memcpy(dp[1] + l, dp[2] + l, sizeof(DP_TYPE) * (r - l + 3));
        last_l = l, last_r = r;
    }
    return dp[2][nb];
}
int main() {
    int dp[MAXN];
    while (scanf("%s %s", A, B) == 2) {
        string a = A, b = B;
        int len = lcs_len(a.c_str(), strlen(A), b.c_str(), strlen(B), dp);
        printf("%d\n", len);
    }
    return 0;
}
