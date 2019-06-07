
#include "utils.h"
#include "CMSketch.h"
#include "UBSketch.h"
#include "CUSketch.h"
#include "Level_BS.h"
#include <fstream>


using namespace std;

struct PTestCase
{
	virtual void experiment(vector<string> &data, unordered_map<string, uint32_t>& ground, vector<double>& result) = 0;

};

template<class T>
struct TestCase : public PTestCase
{
	T *algo;
	TestCase() {
		algo = new T();
	}
	void experiment(vector<string> &data, unordered_map<string, uint32_t>& ground, vector<double>& result) {
		algo->clear();
		algo->exp_res(data, ground, result);
	}
};

int main() {

	const char* filename = "D:\\data_subset\\new_zipf\\030.dat";
	vector<string> data;
	unordered_map<string, uint32_t> ground;
	load_data(data, ground, filename);
	const int memory_in_MB = 1;
	int memory_in_byte = memory_in_MB * 1000000;
	const int hash_num_row = 1;
	const int hash_num_all = 5;
	vector<pair<string , PTestCase * >> tcs = {
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 1, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 1, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 1, 5, hash_num_row, hash_num_all>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 2, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 2, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 2, 5, hash_num_row, hash_num_all>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 3, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 3, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 3, 5, hash_num_row, hash_num_all>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 4, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 4, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 4, 5, hash_num_row, hash_num_all>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_MB * 5, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_MB * 5, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_MB * 5, 5, hash_num_row, hash_num_all>>()),
	};

	vector<double> result;
	cout << "ARE_Memory(MB), CM, CU, UB" << endl;
	int algo_num = 3;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % algo_num == 0) {
			cout << double(memory_in_byte) / 1000000 * ((algo_num + i) / algo_num);
		}
		cout << ",";
		
		tcs[i].second->experiment(data, ground, result);
		cout << result[1];
		if ((i + 1) % algo_num == 0) {
			cout << endl;
		}
	}


	ofstream aae_file;
	aae_file.open("aae.txt");
	aae_file << "Memory(MB), CM, CU, UB" << endl;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % algo_num == 0) {
			aae_file << double(memory_in_byte) / 1000000 * ((algo_num + i) / algo_num);
		}
		aae_file << ",";
		tcs[i].second->experiment(data, ground, result);
		aae_file << result[0];
		if ((i + 1) % algo_num == 0) {
			aae_file << endl;
		}
	}

	ofstream are_file;
	are_file.open("are.txt");
	are_file << "Memory(MB), CM, CU, UB" << endl;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % algo_num == 0) {
			are_file << double(memory_in_byte) / 1000000 * ((algo_num + i) / algo_num);
		}
		are_file << ",";
		tcs[i].second->experiment(data, ground, result);
		are_file << result[1];
		if ((i + 1) % algo_num == 0) {
			are_file << endl;
		}
	}

	ofstream pre_file;
	pre_file.open("pre.txt");
	pre_file << "Memory(MB), CM, CU, UB" << endl;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % algo_num == 0) {
			pre_file << double(memory_in_byte) / 1000000 * ((algo_num + i) / algo_num);
		}
		pre_file << ",";
		tcs[i].second->experiment(data, ground, result);
		pre_file << result[2];
		if ((i + 1) % algo_num == 0) {
			pre_file << endl;
		}
	}

	system("pause");



}

