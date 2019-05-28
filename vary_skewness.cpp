
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
	
	vector<pair<double, string>> skew_file = {
		make_pair(0.0, "D:\\data_subset\\new_zipf\\000.dat"),
		make_pair(0.3, "D:\\data_subset\\new_zipf\\003.dat"),
		make_pair(0.6, "D:\\data_subset\\new_zipf\\006.dat"),
		make_pair(0.9, "D:\\data_subset\\new_zipf\\009.dat"),
		make_pair(1.2, "D:\\data_subset\\new_zipf\\012.dat"),
		make_pair(1.5, "D:\\data_subset\\new_zipf\\015.dat"),
		make_pair(1.8, "D:\\data_subset\\new_zipf\\018.dat"),
		make_pair(2.1, "D:\\data_subset\\new_zipf\\021.dat"),
		make_pair(2.4, "D:\\data_subset\\new_zipf\\024.dat"),
		make_pair(2.7, "D:\\data_subset\\new_zipf\\027.dat"),
		make_pair(3.0, "D:\\data_subset\\new_zipf\\030.dat"),
	};

	const int memory_in_byte = 6000000;
	vector<pair<string, PTestCase * >> tcs = {
		make_pair("CM" ,new TestCase<CMSketch<memory_in_byte, 3>>()),
		make_pair("CU" ,new TestCase<CUSketch<memory_in_byte, 3>>()),
		make_pair("UB" ,new TestCase<UBSketch<memory_in_byte, 5>>()),
	};

	ofstream aae_file;
	aae_file.open("aae.txt");
	aae_file << "Skewness, CM, CU, UB" << endl;
	for (int i = 0; i < skew_file.size(); i++) {
		cout << "start skew " << skew_file[i].first << endl;
		aae_file << skew_file[i].first << ",";
		vector<string> data;
		unordered_map<string, uint32_t> ground;
		load_data(data, ground, skew_file[i].second.c_str());
		vector<double> result;
		tcs[0].second->experiment(data, ground, result);
		aae_file << result[0] << ",";
		tcs[1].second->experiment(data, ground, result);
		aae_file << result[0] << ",";
		tcs[2].second->experiment(data, ground, result);
		aae_file << result[0] << endl;
	}

	ofstream are_file;
	are_file.open("are.txt");
	are_file << "Skewness, CM, CU, UB" << endl;
	for (int i = 0; i < skew_file.size(); i++) {
		cout << "start skew " << skew_file[i].first << endl;
		are_file << skew_file[i].first << ",";
		vector<string> data;
		unordered_map<string, uint32_t> ground;
		load_data(data, ground, skew_file[i].second.c_str());
		vector<double> result;
		tcs[0].second->experiment(data, ground, result);
		are_file << result[1] << ",";
		tcs[1].second->experiment(data, ground, result);
		are_file << result[1] << ",";
		tcs[2].second->experiment(data, ground, result);
		are_file << result[1] << endl;
	}

	ofstream pre_file;
	pre_file.open("pre.txt");
	pre_file << "Skewness, CM, CU, UB" << endl;
	for (int i = 0; i < skew_file.size(); i++) {
		cout << "start skew " << skew_file[i].first << endl;
		pre_file << skew_file[i].first << ",";
		vector<string> data;
		unordered_map<string, uint32_t> ground;
		load_data(data, ground, skew_file[i].second.c_str());
		vector<double> result;
		tcs[0].second->experiment(data, ground, result);
		pre_file << result[2] << ",";
		tcs[1].second->experiment(data, ground, result);
		pre_file << result[2] << ",";
		tcs[2].second->experiment(data, ground, result);
		pre_file << result[2] << endl;
	}



	system("pause");



}

