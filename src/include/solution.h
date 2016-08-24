#ifndef SOLUTION_H
#define SOLUTION_H

#include <complex>
#include <cstddef>
#include <array>
#include <vector>
#include <ostream>

#include <Eigen/Eigen>
#include "fmt/format.h"

#include "node.h"
#include "element.h"
#include "material.h"
#include "section.h"
#include "boundary.h"
#include "sparse_matrix.h"
#include "mesh_reader.h"
#include "eigenpair.h"

namespace cafea
{
/**
 *  Solution base def.
 */
template <class FileReader, class Scalar=float, class ResultScalar=double>
class SolutionBase {
	public:
		//! Initialize environment.
		virtual void init()=0;
		//! Clear model data.
		virtual void clear()=0;
		//! Load input file.
		virtual void load(const char* fn)=0;
		//! Check input model data.
		virtual void check()=0;
		//! Analyze pattern.
		virtual void analyze()=0;
		//! Assemble global matrix.
		virtual void assembly()=0;
		//! Solve.
		virtual void solve()=0;
		//! Save MAT file.
		virtual void write2mat(const char*)=0;
		//! Post process.
		virtual void post_process()=0;
		//! Get model information.
		virtual std::array<size_t, 5> get_info()const=0;
};

/**
 *  Solution of modal analysis.	
 */
template <class FileReader, class Scalar=float, class ResultScalar=double>
class SolutionModal: public SolutionBase <FileReader, Scalar, ResultScalar>{
	public:
		//! Default constructor.
		SolutionModal(){};
		//! Destructor.
		~SolutionModal() { init();};
		//! Initialize environment.
		void init() override;
		void clear() override {init();};
		//! Load input file.
		void load(const char* file_name) override;
		//! Load input file.
		void load(std::string fn) { load(fn.c_str());};
		//! Check input model data.
		void check() override;
		//! Analyze pattern.
		void analyze() override;
		//! Assemble global matrix.
		void assembly() override;
		//! Solve.
		void solve() override;
		//! Post process.
		void post_process() override;
		//! Save variables to MAT files.
		void write2mat(const char*) override;
		//! Get information.
		std::array<size_t, 5> get_info()const override;
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const SolutionModal &a)
		{
			return cout << "This is solution of modal analysis.\n";
		};
	protected:
		FileReader file_parser_;//!< Input file loader.
		dict_<Material<Scalar>> matl_group_;//!< Material dictionary.
		dict_<Section<Scalar>> sect_group_;//!< Section dictionary.
		std::vector<Boundary<Scalar>> bc_group_;//!< Boundary dictionary.
		
		SparseMat<ResultScalar> mat_pair_;//!< Global stiffness and mass matrix.
		matrix_<ResultScalar> mode_shape_;//!< Mode shape of FEA model.
		matrix_<ResultScalar> natural_freq_;//!< Natural frequencies and errors.
		
		dict_<Node<Scalar, ResultScalar>> node_group_;//!< Node dictionary.
		dict_<Element<ResultScalar>> elem_group_;//!< Element dictionary.	
		
		EigenSolver<ResultScalar> solver_;//!< Generalize Eigenpair solver.
};

/**
 *  Solution of harmonic
 */
/*template <class FileReader, class Scalar=float, class ResultScalar=double>
class SolutionHarmonic: public SolutionBase <FileReader, Scalar, ResultScalar> {
	public:
		//! Default constructor.
		SolutionHarmonic(){};
		//! Destructor.
		~SolutionHarmonic();
		//! Initialize environment.
		void init();
		//! Load input file.
		void load(const char* file_name);
		//! Check input model data.
		void check();
		//! Analyze pattern.
		void analyze();
		//! Assemble global matrix.
		void assembly();
		//! Solve.
		void solve();
		//! Post process.
		void post_process();
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const SolutionHarmonic &a)
		{
			return cout << "This is solution of harmonic analysis.\n";
		};
	protected:
		dict_<Node<Scalar, std::complex<ResultScalar>>> node_group_;//!< Node dictionary.
		dict_<Element<ResultScalar>> elem_group_;//!< Element dictionary.
		
		vecX_<Scalar> damping_;//!< Damping ratio.
		vecX_<Scalar> freq_range_;//!< Frequency range.
		
};*/
//! Specialization with float type.
template class SolutionBase<AnsysCdbReader<float>>;
template class SolutionModal<AnsysCdbReader<float>>;
}
#endif