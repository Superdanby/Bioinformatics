# Bioinformatics

- `dp_openmp.cpp`: a dynamic programming approach to match biological sequences with OpenMP boost. It is modified from [morris821028's code](https://github.com/morris821028/UVa/blob/master/OnlineJudge/JudgeGirl/10110.%20Longest%20Common%20Subsequence%20(OpenMP)/main-3-array.cpp)

## Comparisions

| bandwidth | bandwith_alignment.cpp | bandwith_alignment_xsimd.cpp | bandwith_alignment_openmp_simd | bandwith_alignment_openmp_simd_parallel | bandwith_alignment.cpp -march=native |
| - | - | - | - | - | - |
| 1e5 | 10.089s | 4.109s | 3.223s | 5.531s | 3.107s |
| 1e6 | 9.030s | 3.672s | 2.75s | 4.788s | 2.719s |
