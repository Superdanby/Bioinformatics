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
bandwith_alignment_openmp: bandwith_alignment_openmp.cpp
	g++ -std=c++17 bandwith_alignment_openmp.cpp -O3 -fopenmp -o bandwith_alignment_openmp
bandwith_alignment_xsimd: bandwith_alignment_xsimd.cpp
	g++ -std=c++17 -I./xsimd/include bandwith_alignment_xsimd.cpp -O3 -march=native -o bandwith_alignment_xsimd
test_bandwith_alignment_xsimd: bandwith_alignment_xsimd
	time ./bandwith_alignment_xsimd <<< "ACACA AAA"
	time ./bandwith_alignment_xsimd <<< "ACAGAT GCAGAC"
	time ./bandwith_alignment_xsimd < gap1.dat
	time ./bandwith_alignment_xsimd < gap2.dat
	time ./bandwith_alignment_xsimd < gap3.dat
	time ./bandwith_alignment_xsimd < gap4.dat
	time ./bandwith_alignment_xsimd < test1.dat
	time ./bandwith_alignment_xsimd < test2.dat
	time ./bandwith_alignment_xsimd < test3.dat
bandwith_affine_openmp: bandwith_affine_openmp.cpp
	g++ -std=c++17 bandwith_affine_openmp.cpp -O3 -fopenmp -o bandwith_affine_openmp
test_bandwith_alignment_openmp: bandwith_alignment_openmp
	time ./bandwith_alignment_openmp <<< "ACACA AAA"
	time ./bandwith_alignment_openmp <<< "ACAGAT GCAGAC"
	time ./bandwith_alignment_openmp < gap1.dat
	time ./bandwith_alignment_openmp < gap2.dat
	time ./bandwith_alignment_openmp < gap3.dat
	time ./bandwith_alignment_openmp < gap4.dat
	time ./bandwith_alignment_openmp < test1.dat
	time ./bandwith_alignment_openmp < test2.dat
	time ./bandwith_alignment_openmp < test3.dat
