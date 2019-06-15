#ifndef _CMSKETCH_H
#define _CMSKETCH_H

#include "paras.h"
#include "BOBHash32.h"
#include <string>
#include <iostream>
#include <memory.h>

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
		}
	}

	int query(string key)
	{
		int ret = (1 << 30);
		for (int i = 0; i < d; i++) {
			int tmp = counters[(hash[i]->run(key.c_str(), KEY_LEN)) % w];
			ret = min(ret, tmp);
		}
		return ret;
	}


};

template <int memory_in_bytes, int d>
uint32_t CMSketch<memory_in_bytes, d>::counters[MAX_ARRAY_LEN];

#endif //_CMSKETCH_H