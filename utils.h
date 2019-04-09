#ifndef UTILS_H
#define UTILS_H

#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include "paras.h"

using namespace std;

void load_data(vector<string>& str_data ,unordered_map<string, uint32_t>& ground, const char* filename) {
	FILE *pf = fopen(filename, "rb");
	if (!pf) {
		cerr << filename << "not found." << endl;
		exit(-1);
	}
	ground.clear();
	str_data.clear();
	char key[KEY_LEN];
	uint32_t ret = 0;
	while (fread(key, 1, KEY_LEN, pf)) {
		string str_key(key, KEY_LEN);
		str_data.push_back(str_key);
		ground[str_key]++;
		ret++;
		if (ret == MAX_INSERT_PACKAGE)
			break;
	}
	fclose(pf);

	printf("load %d items, %lu distinct items\n", ret, ground.size());
}

#endif
