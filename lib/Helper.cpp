#include <Helper.h>

string fileBaseName(const string& Fname) {
	return string( std::find_if(Fname.rbegin(), Fname.rend(), MatchSeperator()).base(), Fname.end());
}
