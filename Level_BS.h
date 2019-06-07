#ifndef _LEVEL_BS_H
#define _LEVEL_BS_H

#include "paras.h"
#include "BOBHash32.h"
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

template <int memory_in_MB, int d, int hash_num_row, int hash_num_all>
class LBSketch
{
private:
	vector<int> index_start;
	vector<int> row_length;
	vector<int> counter_size;
	BOBHash32* hash[hash_num_all];
	int memory_in_bytes;
public:
	static int counters[MAX_ARRAY_LEN];
	void clear() {
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
	}
	LBSketch() {
		memory_in_bytes = memory_in_MB * 1000000;
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
		for (int i = 0; i < hash_num_all; i++)
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
	~LBSketch() {
		for (int i = 0; i < hash_num_all; i++)
			delete hash[i];
	}
	/*
	void insert(string key, int f) {
		for (int i = 0; i < d; i++) {
			bool all_overflow = true;
			for (int i_hash = 0; i_hash < hash_num_row; i_hash++) {
				int index = index_start[i] + (hash[i_hash + i * hash_num_row]->run(key.c_str(), KEY_LEN) % row_length[i]);
				counters[index] += f;
				if (counter_size[i] != 32) {
					if (counters[index] >= (1 << counter_size[i])) {
						counters[index] = (1 << counter_size[i]) - 1;
					}
					else {
						all_overflow = false;
					}
				}
			}
			if (all_overflow) {
				continue;
			}
			else {
				break;
			}
		}
	}*/

	
	void insert(string key, int f) {
		for (int i = 0; i < d; i++) {
			int index = index_start[i] + (hash[i]->run(key.c_str(), KEY_LEN) % row_length[i]);
			if (index == 1052044) {
				cout << "debug";
			}
			counters[index] += f;
			if (counter_size[i] != 32) {
				if (counters[index] >= (1 << counter_size[i])) {
					counters[index] = (1 << counter_size[i]) - 1;
					continue;
				}
				else {
					break;
				}
			}
		}
	}


	int query(string key) {
		int ret = 0;
		for (int i = 0; i < d; i++) {
			int index = index_start[i] + (hash[i]->run(key.c_str(), KEY_LEN) % row_length[i]);
			ret = ret + counters[index];
			if (counter_size[i] != 32) {
				if (counters[index] != (1 << counter_size[i]) - 1) {
					if (counters[index] >= (1 << counter_size[i])) {
						cerr << "UB SKETCH ERROR, press Enter." << endl;
						cin.get();
						exit(-1);
					}
					break;
				}
				else {
					continue;
				}
			}
		}
		return ret;
	}

	/*
	int query(string key) {
		int ret = 0;
		for (int i = 0; i < d; i++) {
			bool all_overflow = true;
			for (int i_hash = 0; i_hash < hash_num_row; i_hash++) {
				int index = index_start[i] + (hash[i_hash + i * hash_num_row]->run(key.c_str(), KEY_LEN) % row_length[i]);
				if (counter_size[i] != 32) {
					if (counters[index] != (1 << counter_size[i]) - 1) {
						if (counters[index] >= (1 << counter_size[i])) {
							cerr << "UB SKETCH ERROR, press Enter." << endl;
							cin.get();
							exit(-1);
						}
						all_overflow = false;
						ret = ret + counters[index];
						break;
					}
				}
				else {
					ret = ret + counters[index];
					break;
				}
			}
			if (all_overflow) {
				int index = index_start[i] + (hash[0 + i * hash_num_row]->run(key.c_str(), KEY_LEN) % row_length[i]);
				ret = ret + counters[index];
			}
			else {
				break;
			}
		}

		return ret;
	}*/

	void exp_res(vector<string>& data, unordered_map<string, uint32_t>& ground, vector<double>& result) {
		for (auto key : data) {
			insert(key, 1);
		}

		double aae = 0;
		double are = 0;
		double precision = 0;
		/******debug******/
		int key_index = 0;
		for (auto key : ground) {
			if (key_index == 99) {
				cout << "debug";
			}
			int q_ret = query(key.first);
			int true_ret = ground[key.first];
			if (q_ret < true_ret) {
				cerr << "LB SKETCH ERROR" << endl;
				exit(-1);
			}
			aae = aae + q_ret - true_ret;
			are = are + (q_ret - true_ret) / true_ret;
			if (q_ret == true_ret) {
				precision++;
			}

			key_index++;
		}
		aae = aae / ground.size();
		are = are / ground.size();
		precision = precision / ground.size();
		result.clear();
		result.push_back(aae); result.push_back(are); result.push_back(precision);

		//cout << "UB: AAE:" << aae << ", ARE:" << are << endl;
	}
};

template <int memory_in_MB, int d, int hash_num_row, int hash_num_all>
int LBSketch<memory_in_MB, d, hash_num_row, hash_num_all>::counters[MAX_ARRAY_LEN];

#endif // !_LEVEL_BS_H

