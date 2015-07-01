/*Find basic exception types information here: http://stdcxx.apache.org/doc/stdlibref/2-3.html*/
#include <stdexcept>
#include <string>

using namespace std;

class InvalidDataException : public domain_error {
public:
	InvalidDataException(string msg) : domain_error(msg) {}
};

class UpdateLengthMismatchException : public logic_error {
public:
	UpdateLengthMismatchException(string msg) : logic_error(msg) {}
};