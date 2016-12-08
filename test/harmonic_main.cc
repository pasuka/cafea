#include "cafea.h"

namespace
{
using cafea::FEModelReader;
using cafea::SolutionBase;
using cafea::SolutionHarmonicFull;

//! Simplify.
using SolutionSimple = SolutionHarmonicFull<FEModelReader<float>, float, double>;	
}
int main(int argc, char *argv[])
{
	if(2>argc){
		fmt::print("None of input files.\n");
		return -1;
	}
	else{
		
		// FEModelReader<float> file_parser;
		// std::string fn = argv[1];
		// file_parser.load_model(fn);
		// file_parser.clean_model();
		
		fmt::print("Begin.\n");
		std::unique_ptr<SolutionBase<FEModelReader<float>>> example(new SolutionSimple);
		fmt::print("Init.\n");
		example->init();
		fmt::print("Load input file:{}\n", argv[1]);
		example->load(argv[1]);
		fmt::print("Analyze\n");
		example->analyze();
		fmt::print("Assembly\n");
		example->assembly();
		fmt::print("Solve\n");
		example->solve();
		fmt::print("Post process\n");
		example->post_process();
		if(2<argc)example->write2mat(argv[2]);
		// example->write2mat("./tset_harmonic.mat");
		
	}
	return 0;
}
