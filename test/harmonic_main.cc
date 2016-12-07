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
		/*
		FEModelReader<float> file_parser;
		std::string fn = argv[1];
		file_parser.load_model(fn);
		file_parser.clean_model();
		*/
		std::unique_ptr<SolutionBase<FEModelReader<float>>> example(new SolutionSimple);
		// SolutionBase<FEModelReader<float>> *example(new SolutionSimple);
		example->init();
		example->load(argv[1]);
		example->analyze();
		example->assembly();
		example->solve();
		example->post_process();
		example->write2mat("./tset_harmonic.mat");
	}
	return 0;
}
