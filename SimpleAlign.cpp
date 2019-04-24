#include <cstdio>
#include <cstring>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <vector>
#include "xsimd/xsimd.hpp"

using namespace std;

namespace xs = xsimd;
using vector_int = std::vector<int, xsimd::aligned_allocator<int, XSIMD_DEFAULT_ALIGNMENT>>;

void run(const std::string& a, const std::string& b, vector_int& res)
{
    std::size_t size = res.size();
    std::size_t simd_size = xsimd::simd_type<int>::size;
    std::cout << "size: " << simd_size << "\n";
    std::size_t vec_size = size - size % simd_size;
    xs::simd_type<int> match(8), mismatch(-5);
    xs::simd_type<int> add = match - mismatch;

    vector_int A(a.begin(), a.end()), B(b.begin(), b.end());

    for(std::size_t i = 0; i < vec_size; i += simd_size)
    {
        auto ba = xs::load_aligned(&A[i]);
        auto bb = xs::load_aligned(&B[i]);
        auto bres = xs::simd_type<int>(ba == bb);
        bres = bres & add;
        bres = bres + mismatch;
        bres.store_aligned(&res[i]);
        // std::cout << bres << "\n";
    }
    for(std::size_t i = vec_size; i < size; ++i)
    {
        res[i] = (a[i] == b[i]) * 13 - 5;
    }

    return;
}

const int size=100;
static char A[size], B[size];
int main(){
	// scanf("%s %s", A, B);
    std::string A, B;
    std::cin >> A >> B;
    vector_int res (max(A.size(), B.size()));
    run(A, B, res);

    for (const auto &x: res)
        std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
