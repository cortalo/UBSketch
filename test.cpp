#include "utils.h"
#include "CMSketch.h"
#include "UBSketch.h"
#include "CUSketch.h"


using namespace std;


struct PTestCase
{
	virtual void experiment(vector<string> &data, unordered_map<string, uint32_t>& ground, vector<double>& result) = 0;

};

template<class T>
struct TestCase: public PTestCase
{
	T *algo;
	TestCase() {
		algo = new T();
	}
	void experiment(vector<string> &data, unordered_map<string, uint32_t>& ground, vector<double>& result) {
		algo->exp_res(data, ground, result);
	}
};

int main() {

	const char* filename = "D:\\data_subset\\formatted00.dat";
	vector<string> data;
	unordered_map<string, uint32_t> ground;
	load_data(data, ground, filename);
	const int memory_in_byte = 100000;
	vector<PTestCase * > tcs = {
		new TestCase<CMSketch<memory_in_byte, 3>>(),
		new TestCase<CUSketch<memory_in_byte, 3>>(),
		new TestCase<UBSketch<memory_in_byte, 4>>(),
	};

	vector<double> result;
	for (int i = 0; i < tcs.size(); i++) {
		tcs[i]->experiment(data, ground, result);
	}


	system("pause");



}

