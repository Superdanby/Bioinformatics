.PHONY: test_dp_openmp
dp_openmp: dp_openmp.cpp
	g++ -std=c++17 dp_openmp.cpp -O3 -fopenmp -o dp_openmp
test_dp_openmp: dp_openmp
	./dp_openmp < dp_openmp.in
