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
