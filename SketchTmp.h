#ifndef SKETCHTMP_H
#define SKERCHTMP_H

#include "paras.h"
#include "BOBHash32.h"
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <time.h>

class SKETCHTMP
{
public:
	virtual void insert(string key, int f);
	virtual int query(string key);
	double speedtest(vector<string>& data) {
		double dur;
		clock_t 
		for (auto key : data) {
			insert(key, 1);
		}
	}

};

#endif // !SKETCHTMP_H
