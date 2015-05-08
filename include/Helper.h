#ifndef HELPER_H
#define HELPER_H

#include <algorithm>

using namespace std;

struct MatchSeperator {
	bool operator()(char c) {
		return c=='/';
	}
};

string fileBaseName(const string& Fname) {
	return string( std::find_if(Fname.rbegin(), Fname.rend(), MatchSeperator()).base(), Fname.end());
}

#endif