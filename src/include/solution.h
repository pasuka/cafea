#ifndef SOLUTION_H
#define SOLUTION_H

#include <complex>
#include <vector>
#include <ostream>

#include <Eigen/Eigen>

#include "node.h"
#include "element.h"
#include "material.h"
#include "section.h"
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
		virtual ~SolutionBase()=0;
		//! Initialize environment.
		virtual void init();
		//! Clear model data.
		virtual void clear();
		//! Load input file.
		virtual void load(const char* fn);
		//! Check input model data.
		virtual void check();
		//! Assemble global matrix.
		virtual void assembly();
		//! Solve.
		virtual void solve();
		//! Post process.
		virtual void post_process();
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const SolutionBase &a)
		{
			return cout << "This is base of solution.\n";
		};
	protected:
		FileReader file_parser_;//!< Input file loader.
		dict_<Material<Scalar>> matl_group_;//!< Material dictionary.
		dict_<Section<Scalar>> sect_group_;//!< Section dictionary.
		std::vector<Boundary<Scalar>> bc_group_;//!< Boundary dictionary.
		
		SparseMat<ResultScalar> mat_pair_;//!< Global stiffness and mass matrix.
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
		~SolutionModal() { init();};
		//! Initialize environment.
		void init();
		//! Load input file.
		void load(const char* file_name);
		//! Check input model data.
		void check();
		//! Assemble global matrix.
		void assembly();
		//! Solve.
		void solve();
		//! Post process.
		void post_process();
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const SolutionModal &a)
		{
			return cout << "This is solution of modal analysis.\n";
		};
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
		~SolutionHarmonic();
		//! Initialize environment.
		void init();
		//! Load input file.
		void load(const char* file_name);
		//! Check input model data.
		void check();
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
		dict_<Element<ResultScalar, std::complex<ResultScalar>>> elem_group_;//!< Element dictionary.
		
		vecX_<Scalar> damping_;//!< Damping ratio.
		vecX_<Scalar> freq_range_;//!< Frequency range.
		
};
}
#endif