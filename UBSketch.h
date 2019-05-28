#ifndef _UBSKETCH_H
#define _UBSKETCH_H

#include "paras.h"
#include "BOBHash32.h"
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

template <int memory_in_bytes, int d>
class UBSketch
{
private:
	vector<int> index_start;
	vector<int> row_length;
	vector<int> counter_size;
	BOBHash32 * hash[d];
public:
	static int counters[MAX_ARRAY_LEN];
	void clear() {
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
	}
	UBSketch() {
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
		for (int i = 0; i < d; i++)
			hash[i] = new BOBHash32(i + 750);
		counter_size.clear();
		counter_size.push_back(2);
		for (int i = 1; i < d; i++) {
			counter_size.push_back(counter_size.back() * 2);
		}
		index_start.clear();
		index_start.push_back(0);
		for (int i = 1; i < d; i++) {
			index_start.push_back(index_start.back() + (memory_in_bytes * 8 / d / counter_size[i - 1]));
		}
		row_length.clear();
		for (int i = 0; i < d; i++) {
			row_length.push_back(memory_in_bytes * 8 / d / counter_size[i]);
		}
		
	}
	~UBSketch() {
		for (int i = 0; i < d; i++)
			delete hash[i];
	}
	
	void insert(string key, int f) {
		for (int i = 0; i < d; i++) {
			int index = index_start[i] + (hash[i]->run(key.c_str(), KEY_LEN) % row_length[i]);
			counters[index] += f;
			if (counter_size[i] != 32) {
				if (counters[index] >= (1 << counter_size[i])) {
					counters[index] = (1 << counter_size[i]) - 1;
				}
			}
		}
	}
	
	int query(string key) {
		int ret = INT_MAX;
		for (int i = 0; i < d; i++) {
			int index = index_start[i] + (hash[i]->run(key.c_str(), KEY_LEN) % row_length[i]);
			if (counter_size[i] != 32) {
				if (counters[index] != (1 << counter_size[i]) - 1) {
					ret = min(ret, counters[index]);
					if (counters[index] >= (1 << counter_size[i])) {
						cerr << "UB SKETCH ERROR, press Enter." << endl;
						cin.get();
						exit(-1);
					}
				}
			}
			else {
				ret = min(ret, counters[index]);
			}
		}
		if (ret == INT_MAX) {
			ret = (1 << counter_size[d]) - 1;
		}
		return ret;
	}

	void exp_res(vector<string> &data, unordered_map<string, uint32_t>& ground, vector<double>& result) {
		for (auto key : data) {
			insert(key, 1);
		}

		double aae = 0;
		double are = 0;
		double precision = 0;
		for (auto key : ground) {
			int q_ret = query(key.first);
			int true_ret = ground[key.first];
			if (q_ret < true_ret) {
				cerr << "UB SKETCH ERROR" << endl;
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

		//cout << "UB: AAE:" << aae << ", ARE:" << are << endl;
	}
};

template <int memory_in_bytes, int d>
int UBSketch<memory_in_bytes, d>::counters[MAX_ARRAY_LEN];

#endif // !_UBSKETCH_H

