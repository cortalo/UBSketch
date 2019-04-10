#include "utils.h"
#include "CMSketch.h"
#include "UBSketch.h"
#include "CUSketch.h"

using namespace std;
int main() {

	const char* filename = "C:\\Users\\Corta\\OneDrive\\XPS\\Document\\1806\\data_subset\\formatted00.dat";
	vector<string> data;
	unordered_map<string, uint32_t> ground;
	load_data(data, ground, filename);

	const int memory_in_byte = 50000;
	
	CMSketch<memory_in_byte, 6> cm_sketch;
	vector<double> cm_result;
	cm_sketch.exp_res(data, ground, cm_result);

	CUSketch<memory_in_byte, 6> cu_sketch;
	vector<double> cu_result;
	cu_sketch.exp_res(data, ground, cm_result);

	UBSketch<memory_in_byte, 3> ub_sketch;
	vector<double> ub_result;
	ub_sketch.exp_res(data, ground, ub_result);



	system("pause");



}

