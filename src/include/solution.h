#ifndef SOLUTION_H
#define SOLUTION_H

#include <complex>
#include <vector>
#include <unordered_map>

#include <Eigen/Eigen>

#include "node.h"
#include "element.h"
#include "material.h"
#include "boundary.h"
#include "sparse_matrix.h"

namespace cafea
{
/**
 *  Solution base definition.	
 */
template <class FileReader, class ResultDataType>
class SolutionBase {
	public:
		//! Default constructor.
		SolutionBase(){};
		//! Destructor.
		virtual ~SolutionBase();
		//! Initialize environment.
		virtual void init_model() const=0;
		//! Load input file.
		virtual void load_file(const std::string& file_name) const=0;
		//! Load input file.
		virtual void load_file(const char* file_name) const=0;
		//! Check input model data.
		virtual void check_model() const=0;
		//! Analyze sparse pattern.
		virtual void analyze_pattern() const=0;
		//! Assemble global matrix.
		virtual void assemble_matrix() const=0;
	protected:
		template <typename T>
		using dict_ = std::unordered_map<int, T>;
		dict_<Node<ResultDataType>> m_node_dict;//!< Node dictionary.
		dict_<Element<ResultDataType>> m_elem_dict;//!< Element dictionary.
		dict_<Material> m_matl_dict;//!< Material dictionary.
		std::vector<Boundary> m_boundary_dict;//!< Boundary dictionary.
		
		FileReader m_file_parser;//!< Input file loader.
};

/**
 *  Solution for response in frequency domain.
 */
template <class FileReader>
class SolutionFrequencyResponse: public SolutionBase <FileReader, std::complex<double>> {
	public:
		//! Default constructor.
		SolutionFrequencyResponse(){};
		//! Destructor.
		~SolutionFrequencyResponse(){
			m_node_dict.clear();
			m_elem_dict.clear();
			m_matl_dict.clear();
			m_boundary_dict.clear();
		};
		//! Initialize environment.
		void init_model();
		//! Load input file.
		void load_file(const std::string& file_name);
		//! Load input file.
		void load_file(const char* file_name);
		//! Check model data.
		void check_model();
		//! Analyze sparse pattern.
		void analyze_pattern();
		//! Assemble global matrix.
		void assemble_matrix();
		//! Solve.
		void solve();
		//! Post process.
		void post_process();
		//! Set mass matrix as lumped.
		void set_lump(bool x){m_lump = x};
		//! Get mass matrix whether lumped.
		bool get_lump() const {return m_lump;};
	private:
		SparseMat<double> m_global_matrix;//!< Global matrix index and values.
		bool m_lump{false};//!< Lump mass matrix.
};

/**
 * Solution for response in time domain. 
 */
template <class FileReader>
class SolutionTimeResponse: public SolutionBase<FileReader, double> {
	public:
		//! Default constructor.
		SolutionTimeResponse(){};
		//! Destructor.
		~SolutionTimeResponse(){
			m_node_dict.clear();
			m_elem_dict.clear();
			m_matl_dict.clear();
			m_boundary_dict.clear();
		};
		//! Initialize environment.
		void init_model();
		//! Load input file.
		void load_file(const std::string& file_name);
		//! Load input file.
		void load_file(const char* file_name);
		//! Check model data.
		void check_model();
		//! Analyze sparse pattern.
		void analyze_pattern();
		//! Assemble global matrix.
		void assemble_matrix();
		//! Solve.
		void solve();
		//! Post process.
		void post_process();
		//! Set mass matrix as lumped.
		void set_lump(bool x){m_lump = x};
		//! Get mass matrix whether lumped.
		bool get_lump() const {return m_lump;};
	private:
		SparseMat<double> m_global_matrix;//!< Global matrix index and values.
		bool m_lump{false};//!< Lump mass matrix.
};

}
#endif