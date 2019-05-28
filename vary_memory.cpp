
#include "utils.h"
#include "CMSketch.h"
#include "UBSketch.h"
#include "CUSketch.h"
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

	const char* filename = "D:\\data_subset\\webdocs_form00.dat";
	vector<string> data;
	unordered_map<string, uint32_t> ground;
	load_data(data, ground, filename);
	const int memory_in_byte = 200000;
	vector<pair<string , PTestCase * >> tcs = {
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 1, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 1, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 1, 5>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 2, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 2, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 2, 5>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 3, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 3, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 3, 5>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 4, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 4, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 4, 5>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 5, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 5, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 5, 5>>()),
	};

	vector<double> result;
	cout << "ARE_Memory(MB), CM, CU, UB" << endl;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % 3 == 0) {
			cout << double(memory_in_byte) / 1000000 * ((3 + i) / 3);
		}
		cout << ",";
		
		tcs[i].second->experiment(data, ground, result);
		cout << result[1];
		if ((i + 1) % 3 == 0) {
			cout << endl;
		}
	}


	ofstream aae_file;
	aae_file.open("aae.txt");
	aae_file << "Memory(MB), CM, CU, UB" << endl;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % 3 == 0) {
			aae_file << double(memory_in_byte) / 1000000 * ((3 + i) / 3);
		}
		aae_file << ",";
		tcs[i].second->experiment(data, ground, result);
		aae_file << result[0];
		if ((i + 1) % 3 == 0) {
			aae_file << endl;
		}
	}

	ofstream are_file;
	are_file.open("are.txt");
	are_file << "Memory(MB), CM, CU, UB" << endl;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % 3 == 0) {
			are_file << double(memory_in_byte) / 1000000 * ((3 + i) / 3);
		}
		are_file << ",";
		tcs[i].second->experiment(data, ground, result);
		are_file << result[1];
		if ((i + 1) % 3 == 0) {
			are_file << endl;
		}
	}

	ofstream pre_file;
	pre_file.open("pre.txt");
	pre_file << "Memory(MB), CM, CU, UB" << endl;
	for (int i = 0; i < tcs.size(); i++) {
		if (i % 3 == 0) {
			pre_file << double(memory_in_byte) / 1000000 * ((3 + i) / 3);
		}
		pre_file << ",";
		tcs[i].second->experiment(data, ground, result);
		pre_file << result[2];
		if ((i + 1) % 3 == 0) {
			pre_file << endl;
		}
	}

	system("pause");



}

