#include "cafea.h"

namespace
{
using cafea::SolutionBase;
using cafea::SolutionModal;
using cafea::AnsysCdbReader;
using cafea::BcyReader;
//! Simplify.
using SolutionSimple = SolutionModal<AnsysCdbReader<float>, float, double>;	
using SolutionSimple2 = SolutionModal<BcyReader<float>, float, double>;
}
int main(int argc, char *argv[])
{
	if(2>argc){
		fmt::print("None of input files.\n");
		return -1;
	}
	else{
		std::unique_ptr<SolutionBase<BcyReader<float>>> example(new SolutionSimple2);
		example->init();
		example->load(argv[1]);
		example->analyze();
		example->assembly();
		// example->set_parameter(cafea::SolutionOption::MODAL_FREQ_RANGE, {0.0, 1.0e2});
		example->set_parameter(cafea::SolutionOption::MODAL_NUMBER, {30, });
		example->solve();
		cafea::matrix_<double> rst = example->get_result();
		fmt::print("***************************************************************************\n");
		for(int i=0; i<rst.rows(); i++){
			fmt::print("\t{:02d}\t{:7.3f}Hz\t{:7.3f}Hz\n", i+1, rst(i, 0), rst(i, 1));
		}
		fmt::print("***************************************************************************\n");
		if(2<argc)example->write2mat(argv[2]);
	}
	return 0;
}
