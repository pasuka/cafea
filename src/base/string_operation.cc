#include "utils.h"

using std::string;
using std::stoi;
using std::stof;

namespace cafea
{
//! Starts with special string.
bool startswith(std::string s1, std::string header)
{
	return 0==s1.compare(0, header.size(), header);
};

//! Convert to integer.
int str2int(std::string a)
{
	if(a.empty()){
		return -1;
	}
	else{
		return std::stoi(a);
	}
};

//! Convert to float.
float str2float(std::string a)
{
	if(a.empty()){
		return -1.0f;
	}
	else{
		return std::stof(a);
	}
};
}