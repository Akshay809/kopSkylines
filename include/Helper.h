#ifndef HELPER_H
#define HELPER_H

#include <algorithm>

using namespace std;

struct MatchSeperator {
	bool operator()(char c) {
		return c=='/';
	}
};

string fileBaseName(const string&);

#endif