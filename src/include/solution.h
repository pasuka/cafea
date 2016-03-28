#ifndef SOLUTION_H
#define SOLUTION_H

#include <complex>
#include <vector>

#include <Eigen/Eigen>

#include "node.h"
#include "element.h"
#include "material.h"
#include "boundary.h"
#include "sparse_matrix.h"

namespace cafea
{
/**
 *  Solution base def.
 */
template <class FileReader, class Scalar=float>
class SolutionBase {
	public:
		//! Default constructor.
		SolutionBase(){};
		//! Destructor.
		virtual ~SolutionBase(){};
		//! Initialize environment.
		virtual init_model();
		//! Load input file.
		virtual void load_file(const char* file_name);
		//! Check input model data.
		virtual void check_model();
		//! Analyze sparse pattern.
		virtual void analyze_pattern();
		//! Assemble global matrix.
		virtual void assemble_matrix();
		//! Solve.
		virtual void solve();
		//! Post process.
		virtual void post_process();
	protected:
		FileReader<Scalar> file_parser_;//!< Input file loader.
};

/**
 *  Solution of modal analysis.	
 */
template <class FileReader, class Scalar=float, class ResultScalar=double>
class SolutionModal: public SolutionBase <FileReader, Scalar>{
	public:
		//! Default constructor.
		SolutionBase(){};
		//! Destructor.
		~SolutionBase();
		//! Initialize environment.
		void init_model();
		//! Load input file.
		void load_file(const char* file_name);
		//! Check input model data.
		void check_model();
		//! Analyze sparse pattern.
		void analyze_pattern();
		//! Assemble global matrix.
		void assemble_matrix();
		//! Solve.
		void solve();
		//! Post process.
		void post_process();
	protected:
		dict_<Node<Scalar, ResultScalar>> node_group_;//!< Node dictionary.
		dict_<Element<ResultScalar>> elem_group_;//!< Element dictionary.
		dict_<Material<Scalar>> matl_group_;//!< Material dictionary.
		std::vector<Boundary> boundary_group_;//!< Boundary dictionary.
		
		SparseMat<ResultScalar> stif_global_;//!< Global stiffness matrix.
		SparseMat<ResultScalar> mass_global_;//!< Global mass matrix.
};

/**
 *  Solution of harmonic
 */
template <class FileReader, class Scalar=float, class ResultScalar=double>
class SolutionHarmonic: public SolutionBase <FileReader, Scalar>{
	public:
		//! Default constructor.
		SolutionHarmonic(){};
		//! Destructor.
		~SolutionHarmonic(){};
		//! Initialize environment.
		void init_model();
		//! Load input file.
		void load_file(const char* file_name);
		//! Check input model data.
		void check_model();
		//! Analyze sparse pattern.
		void analyze_pattern();
		//! Assemble global matrix.
		void assemble_matrix();
		//! Solve.
		void solve();
		//! Post process.
		void post_process();
	protected:
		dict_<Node<Scalar, std::complex<ResultScalar>>> node_group_;//!< Node dictionary.
		dict_<Element<std::complex<ResultScalar>>> elem_group_;//!< Element dictionary.
		dict_<Material<Scalar>> matl_group_;//!< Material dictionary.
		std::vector<Boundary> boundary_group_;//!< Boundary dictionary.
		
		SparseMat<ResultScalar> stif_global_;//!< Global stiffness matrix.
		SparseMat<ResultScalar> mass_global_;//!< Global mass matrix.
};
}
#endif