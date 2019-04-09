#include "utils.h"
#include "CMSketch.h"

using namespace std;
int main() {

	const char* filename = "C:\\Users\\Corta\\OneDrive\\XPS\\Document\\1806\\data_subset\\formatted00.dat";
	vector<string> data;
	unordered_map<string, uint32_t> ground;
	load_data(data, ground, filename);
	
	CMSketch<1000000, 4> cm_sketch;
	vector<double> cm_result;
	cm_sketch.exp_res(data, ground, cm_result);

	system("pause");

}

