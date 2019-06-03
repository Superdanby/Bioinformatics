.PHONY: test_dp_openmp test_affine_gap_openmp
dp_openmp: dp_openmp.cpp
	g++ -std=c++17 dp_openmp.cpp -O3 -fopenmp -o dp_openmp
test_dp_openmp: dp_openmp
	./dp_openmp < dp_openmp.in
affine_gap_openmp: affine_gap_openmp.cpp
	g++ -std=c++17 affine_gap_openmp.cpp -O3 -fopenmp -o affine_gap_openmp
test_affine_gap_openmp: affine_gap_openmp
	./affine_gap_openmp < gap1.dat
	./affine_gap_openmp < gap2.dat
	./affine_gap_openmp < gap3.dat
	./affine_gap_openmp < gap4.dat
bandwidth_alignment_openmp: bandwidth_alignment_openmp.cpp
	g++ -std=c++17 bandwidth_alignment_openmp.cpp -O3 -march=native -fopenmp -o bandwidth_alignment_openmp
bandwidth_alignment_openmp_simd: bandwidth_alignment_openmp_simd.cpp
	g++ -std=c++17 bandwidth_alignment_openmp_simd.cpp -O3 -march=native -fopenmp -o bandwidth_alignment_openmp_simd
bandwidth_alignment_openmp_simd_parallel: bandwidth_alignment_openmp_simd_parallel.cpp
	g++ -std=c++17 bandwidth_alignment_openmp_simd_parallel.cpp -O3 -march=native -fopenmp -o bandwidth_alignment_openmp_simd_parallel
bandwidth_alignment: bandwidth_alignment.cpp
	g++ -std=c++17 -I./xsimd/include bandwidth_alignment.cpp -O3 -march=native -o bandwidth_alignment
test_bandwidth_alignment: bandwidth_alignment
	time ./bandwidth_alignment <<< "ACACA AAA"
	time ./bandwidth_alignment <<< "ACAGAT GCAGAC"
	time ./bandwidth_alignment < gap1.dat
	time ./bandwidth_alignment < gap2.dat
	time ./bandwidth_alignment < gap3.dat
	time ./bandwidth_alignment < gap4.dat
	time ./bandwidth_alignment < test1.dat
	time ./bandwidth_alignment < test2.dat
	time ./bandwidth_alignment < test3.dat
bandwidth_alignment_xsimd: bandwidth_alignment_xsimd.cpp
	g++ -std=c++17 -I./xsimd/include bandwidth_alignment_xsimd.cpp -O3 -march=native -o bandwidth_alignment_xsimd
test_bandwidth_alignment_xsimd: bandwidth_alignment_xsimd
	time ./bandwidth_alignment_xsimd <<< "ACACA AAA"
	time ./bandwidth_alignment_xsimd <<< "ACAGAT GCAGAC"
	time ./bandwidth_alignment_xsimd < gap1.dat
	time ./bandwidth_alignment_xsimd < gap2.dat
	time ./bandwidth_alignment_xsimd < gap3.dat
	time ./bandwidth_alignment_xsimd < gap4.dat
	time ./bandwidth_alignment_xsimd < test1.dat
	time ./bandwidth_alignment_xsimd < test2.dat
	time ./bandwidth_alignment_xsimd < test3.dat
bandwidth_affine_openmp: bandwidth_affine_openmp.cpp
	g++ -std=c++17 bandwidth_affine_openmp.cpp -O3 -fopenmp -o bandwidth_affine_openmp
test_bandwidth_alignment_openmp: bandwidth_alignment_openmp
	time ./bandwidth_alignment_openmp <<< "ACACA AAA"
	time ./bandwidth_alignment_openmp <<< "ACAGAT GCAGAC"
	time ./bandwidth_alignment_openmp < gap1.dat
	time ./bandwidth_alignment_openmp < gap2.dat
	time ./bandwidth_alignment_openmp < gap3.dat
	time ./bandwidth_alignment_openmp < gap4.dat
	time ./bandwidth_alignment_openmp < test1.dat
	time ./bandwidth_alignment_openmp < test2.dat
	time ./bandwidth_alignment_openmp < test3.dat
test_bandwidth_alignment_openmp_simd: bandwidth_alignment_openmp_simd
	time ./bandwidth_alignment_openmp_simd <<< "ACACA AAA"
	time ./bandwidth_alignment_openmp_simd <<< "ACAGAT GCAGAC"
	time ./bandwidth_alignment_openmp_simd < gap1.dat
	time ./bandwidth_alignment_openmp_simd < gap2.dat
	time ./bandwidth_alignment_openmp_simd < gap3.dat
	time ./bandwidth_alignment_openmp_simd < gap4.dat
	time ./bandwidth_alignment_openmp_simd < test1.dat
	time ./bandwidth_alignment_openmp_simd < test2.dat
	time ./bandwidth_alignment_openmp_simd < test3.dat
test_bandwidth_alignment_openmp_simd_parallel: bandwidth_alignment_openmp_simd_parallel
	time ./bandwidth_alignment_openmp_simd_parallel <<< "ACACA AAA"
	time ./bandwidth_alignment_openmp_simd_parallel <<< "ACAGAT GCAGAC"
	time ./bandwidth_alignment_openmp_simd_parallel < gap1.dat
	time ./bandwidth_alignment_openmp_simd_parallel < gap2.dat
	time ./bandwidth_alignment_openmp_simd_parallel < gap3.dat
	time ./bandwidth_alignment_openmp_simd_parallel < gap4.dat
	time ./bandwidth_alignment_openmp_simd_parallel < test1.dat
	time ./bandwidth_alignment_openmp_simd_parallel < test2.dat
	time ./bandwidth_alignment_openmp_simd_parallel < test3.dat
final_project: final_project.cpp
	# g++ -std=c++17 final_project.cpp -O2 -o final_project
	g++ -std=c++17 final_project.cpp -O3 -march=native -fopenmp -fopt-info-optall -o final_project
test_final_project: final_project
	time ./final_project <<< "ACACA AAA"
	time ./final_project <<< "ACAGAT GCAGAC"
	time ./final_project < gap1.dat
	time ./final_project < gap2.dat
	time ./final_project < gap3.dat
	time ./final_project < gap4.dat
	time ./final_project < test1.dat
	time ./final_project < test2.dat
	time ./final_project < test3.dat
exe_final_project: final_project
	time ./final_project < T10.dat
	time ./final_project < T100.dat
	time ./final_project < T1000.dat
exe_final_project_to_file: final_project
	time ./final_project < T10.dat > T10.out
	time ./final_project < T100.dat > T100.out
	time ./final_project < T1000.dat > T1000.out
