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
public:
	UBSketch() {
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
		for (int i = 0; i < d; i++)
			hash[i] = new BOBHash32(i + 750);
		counter_size.clear();
		index_start.clear();
		index_start.push_back(0);
		for (int i = 1; i < d; i++) {
			int counter_size = (1 << i);
			index_start.push_back(index_start.back() + (memory_in_bytes * 8 / d / counter_size))
		}
		row_length.clear();
		for (int i = 0; i < d; i++) {
			int counter_size = (1 << (i+1));
			row_length.push_back(memory_in_bytes * 8 / d / counter_size);
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

			if (counters[index] >= (1 << counter_size)) {
				counters[index] = (1 << counter_size) - 1;
			}
		}
	}

	int query(string key) {
		int ret = INT_MAX;
		for (int i = 0; i < d; i++) {
			int index = index_start[i] + (hash[i]->run(key.c_str(), KEY_LEN) % row_length[i]);

			if (counters[index] != ((1 << (i + 1)) - 1)) {
				ret = min(ret, counters[index]);
			}
		}
		if (ret == INT_MAX) {
			ret = (1 << (d)) - 1);
		}
	}

private:
	int counters[MAX_ARRAY_LEN];
	vector<int> index_start;
	vector<int> row_length;
	vector<int> counter_size;
	BOBHash32 * hash[d];
};



#endif // !_UBSKETCH_H

