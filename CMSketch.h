#ifndef _CMSKETCH_H
#define _CMSKETCH_H

#include "paras.h"
#include "BOBHash32.h"
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

template <int memory_in_bytes, int d>
class CMSketch
{
private:
	static constexpr int w = memory_in_bytes * 8 / BITS_PER_BUCKET;
	BOBHash32 * hash[d];
public:
	static uint32_t counters[MAX_ARRAY_LEN];
	void clear() {
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
	}
	CMSketch()
	{
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
		for (int i = 0; i < d; i++)
			hash[i] = new BOBHash32(i + 750);
	}

	void print_basic_info()
	{
		printf("CM sketch\n");
		printf("\tCounters: %d\n", w);
		printf("\tMemory: %.6lfMB\n", w * 4.0 / 1024 / 1024);
	}

	~CMSketch()
	{
		for (int i = 0; i < d; i++)
			delete hash[i];
	}

	void insert(string key, int f)
	{
		for (int i = 0; i < d; i++) {
			int index = (hash[i]->run(key.c_str(), KEY_LEN)) % w;
			counters[index] += f;
			if (BITS_PER_BUCKET != 32)
			{
				if (counters[index] >= (1 << BITS_PER_BUCKET)) {
					counters[index] = (1 << BITS_PER_BUCKET) - 1;
				}
			}
		}
	}

	int query(string key)
	{
		int ret = INT_MAX;
		for (int i = 0; i < d; i++) {
			int tmp = counters[(hash[i]->run(key.c_str(), KEY_LEN)) % w];
			ret = min(ret, tmp);
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
				cerr << "CM SKETCH ERROR" << endl;
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
uint32_t CMSketch<memory_in_bytes, d>::counters[MAX_ARRAY_LEN];

#endif //_CMSKETCH_H