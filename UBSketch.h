#ifndef _UBSKETCH_H
#define _UBSKETCH_H

#include "paras.h"
#include "BOBHash32.h"
#include <string>
#include <iostream>
#include <memory.h>

using namespace std;

template <int memory_in_bytes, int d, int hash_num_row, int hash_num_all>
class UBSketch
{
private:
	vector<int> index_start;
	vector<int> row_length;
	vector<int> counter_size;
	BOBHash32 * hash[hash_num_all];
public:
	static int counters[MAX_ARRAY_LEN];
	void clear() {
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
	}
	UBSketch() {
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
	~UBSketch() {
		for (int i = 0; i < hash_num_all; i++)
			delete hash[i];
	}
	
	void insert(string key, int f) {
		for (int i = 0; i < d; i++) {
			for (int i_hash = 0; i_hash < hash_num_row; i_hash++) {
				int index = index_start[i] + (hash[i_hash + i * hash_num_row]->run(key.c_str(), KEY_LEN) % row_length[i]);
				counters[index] += f;
				if (counter_size[i] != 32) {
					if (counters[index] >= (1 << counter_size[i])) {
						counters[index] = (1 << counter_size[i]) - 1;
					}
				}
			}
		}
	}
	
	int query(string key) {
		int ret = (1 << 30);
		for (int i = 0; i < d; i++) {
			for (int i_hash = 0; i_hash < hash_num_row; i_hash++) {
				int index = index_start[i] + (hash[i_hash + i * hash_num_row]->run(key.c_str(), KEY_LEN) % row_length[i]);
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
		}
		if (ret == (1 << 30)) {
			ret = (1 << counter_size[d]) - 1;
		}
		return ret;
	}

};

template <int memory_in_bytes, int d, int hash_num_row, int hash_num_all>
int UBSketch<memory_in_bytes, d, hash_num_row, hash_num_all>::counters[MAX_ARRAY_LEN];

#endif // !_UBSKETCH_H

