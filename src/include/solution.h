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
template <class FileReader, class Scalar=float, class ResultScalar=double>
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
		//! Set mass matrix in lumped.
		void set_lump(bool val) {lump_ = val;};
		//! Get mass matrix lumped info.
		bool get_lump() const {return lump_;};
	protected:
		bool lump_{false};//!< Lump mass matrix.
		FileReader<Scalar> file_parser_;//!< Input file loader.
		dict_<Material<Scalar>> matl_group_;//!< Material dictionary.
		std::vector<Boundary> boundary_group_;//!< Boundary dictionary.
		
		SparseMat<ResultScalar> mat_global_;//!< Global stiffness and mass matrix.
		matrix_<ResultScalar> mode_shape_;//!< Mode shape of FEA model.
		matrix_<ResultScalar> natural_freq_;//!< Natural frequencies and errors.
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
		~SolutionModal() {init_model();};
		//! Initialize environment.
		void init_model()
		{
			if(!mat_global_.empty())matl_group_.clear();
			if(!boundary_group_.empty())boundary_group_.clear();
			if(!node_group_.empty())node_group_.clear();
			if(!elem_group_.empty())elem_group_.clear();
			mat_global_.init_mat();
			mode_shape_.resize(0, 0);
			natural_freq_.resize(0, 0);
		};
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
		dict_<Element<ResultScalar, ResultScalar>> elem_group_;//!< Element dictionary.	
};

/**
 *  Solution of harmonic
 */
template <class FileReader, class Scalar=float, class ResultScalar=double>
class SolutionHarmonic: public SolutionBase <FileReader, Scalar, ResultScalar> {
	public:
		//! Default constructor.
		SolutionHarmonic(){};
		//! Destructor.
		~SolutionHarmonic() {init_model();};
		//! Initialize environment.
		void init_model()
		{
			if(!mat_global_.empty())matl_group_.clear();
			if(!boundary_group_.empty())boundary_group_.clear();
			if(!node_group_.empty())node_group_.clear();
			if(!elem_group_.empty())elem_group_.clear();
			mat_global_.init_mat();
			mode_shape_.resize(0, 0);
			natural_freq_.resize(0, 0);
			damping_.resize(0);
			freq_range_.resize(0);
		};
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
		dict_<Element<ResultScalar, std::complex<ResultScalar>>> elem_group_;//!< Element dictionary.
		
		vecX_<Scalar> damping_;//!< Damping ratio.
		vecX_<Scalar> freq_range_;//!< Frequency range.
		
};
}
#endif