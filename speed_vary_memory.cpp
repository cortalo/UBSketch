
#include "utils.h"
#include "CMSketch.h"
#include "UBSketch.h"
#include "CUSketch.h"
#include <fstream>
#include <time.h>


using namespace std;

struct PTestCase
{
	virtual void experiment(vector<string>& data, unordered_map<string, uint32_t>& ground, vector<double>& result) = 0;
	virtual double test_insert_speed(vector<string>& data) = 0;
};

template<class T>
struct TestCase : public PTestCase
{
	T* algo;
	TestCase() {
		algo = new T();
	}
	void experiment(vector<string>& data, unordered_map<string, uint32_t>& ground, vector<double>& result) {
		algo->clear();
		algo->exp_res(data, ground, result);
	}
	double test_insert_speed(vector<string>& data) {
		double dur;
		clock_t start, end;
		start = clock();
		for (auto key : data) {
			algo->insert(key, 1);
		}
		end = clock();
		dur = (double)(end - start);
		dur = dur / CLOCKS_PER_SEC;
		double insert_speed = (double)data.size();
		insert_speed = insert_speed / dur / 1000000;
		return insert_speed;
	}
};

int main() {

	const char* filename = "D:\\data_subset\\CAIDA\\formatted00.dat";
	vector<string> data;
	unordered_map<string, uint32_t> ground;
	load_data(data, ground, filename);
	const int memory_in_MB = 1;
	int memory_in_byte = memory_in_MB * 1000000;
	const int hash_num_row = 1;
	const int hash_num_all = 5;
	vector<pair<string, PTestCase* >> tcs = {
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 1, 5>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 1, 5>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 1, 5, hash_num_row, hash_num_all>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 2, 5>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 2, 5>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 2, 5, hash_num_row, hash_num_all>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 3, 5>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 3, 5>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 3, 5, hash_num_row, hash_num_all>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 4, 5>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 4, 5>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 4, 5, hash_num_row, hash_num_all>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 5, 5>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 5, 5>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 5, 5, hash_num_row, hash_num_all>>()),
	};

	int algo_num = 3;



	ofstream speed_file;
	speed_file.open("speed.txt");
	speed_file << "Memory(MB), CM, CU, UB" << endl;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % algo_num == 0) {
			speed_file << double(memory_in_byte) / 1000000 * ((algo_num + i) / algo_num);
		}
		speed_file << ",";
		//tcs[i].second->experiment(data, ground, result);
		speed_file << tcs[i].second->test_insert_speed(data);
		//speed_file << result[0];
		if ((i + 1) % algo_num == 0) {
			speed_file << endl;
		}
	}





}

