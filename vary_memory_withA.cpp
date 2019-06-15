
#include "utils.h"
#include "CMSketch.h"
#include "UBSketch.h"
#include "CUSketch.h"
#include "ASketch.h"
#include <fstream>


using namespace std;

struct PTestCase
{
	virtual void experiment(vector<string>& data, unordered_map<string, uint32_t>& ground, vector<double>& result) = 0;

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
		for (auto key : data) {
			algo->insert(key, 1);
		}
		double aae = 0;
		double are = 0;
		double precision = 0;
		for (auto key : ground) {
			int q_ret = algo->query(key.first);
			int true_ret = ground[key.first];
			if (q_ret < true_ret) {
				cerr << "SKETCH ERROR" << endl;
				exit(-1);
			}
			aae = aae + q_ret - true_ret;
			are = are + (q_ret - true_ret) / true_ret;
			if (q_ret == true_ret) {
				precision++;
			}
		}
		aae = aae / ground.size();
		are = are / ground.size();
		precision = precision / ground.size();
		result.clear();
		result.push_back(aae); result.push_back(are); result.push_back(precision);
	}
};

int main() {

	const char* filename = "D:\\data_subset\\new_zipf\\030.dat";
	vector<string> data;
	unordered_map<string, uint32_t> ground;
	load_data(data, ground, filename);
	const int memory_in_byte = 200000;
	const int hash_num_row = 1;
	const int hash_num_all = 5;
	int algo_num = 4;
	vector<pair<string, PTestCase* >> tcs = {
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 1, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 1, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 1, 5, hash_num_row, hash_num_all>>()),
		make_pair("A" ,new TestCase<ASketch<memory_in_byte * 1, 32, 3>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 2, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 2, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 2, 5, hash_num_row, hash_num_all>>()),
		make_pair("A" ,new TestCase<ASketch<memory_in_byte * 2, 32, 3>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 3, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 3, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 3, 5, hash_num_row, hash_num_all>>()),
		make_pair("A" ,new TestCase<ASketch<memory_in_byte * 3, 32, 3>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 4, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 4, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 4, 5, hash_num_row, hash_num_all>>()),
		make_pair("A" ,new TestCase<ASketch<memory_in_byte * 4, 32, 3>>()),
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte * 5, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte * 5, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte * 5, 5, hash_num_row, hash_num_all>>()),
		make_pair("A" ,new TestCase<ASketch<memory_in_byte * 5, 32, 3>>()),
	};

	vector<double> result;
	cout << "ARE_Memory(MB), CM, CU, UB, A" << endl;
	
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
	aae_file << "Memory(MB), CM, CU, UB, A" << endl;
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
	are_file << "Memory(MB), CM, CU, UB, A" << endl;
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
	pre_file << "Memory(MB), CM, CU, UB, A" << endl;
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

}

