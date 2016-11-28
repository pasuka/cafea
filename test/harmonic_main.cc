#include "cafea.h"

namespace
{
using cafea::FEModelReader;
}
int main(int argc, char *argv[])
{
	if(2>argc){
		fmt::print("None of input files.\n");
		return -1;
	}
	else{
		FEModelReader<float> file_parser;
		std::string fn = argv[1];
		file_parser.load_model(fn);
	}
	return 0;
}
