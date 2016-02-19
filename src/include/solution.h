#ifndef SOLUTION_H
#define SOLUTION_H

#include <complex>
#include <unordered_map>

#include <Eigen/Eigen>

#include "node.h"
#include "element.h"
#include "material.h"

namespace cafea
{
/**
 *  Solution base definition.	
 */
template <class ResultDataType>
class SolutionBase {
	public:
		SolutionBase(){};
		~SolutionBase(){};
	private:
		template <typename T>
		using dict_ = std::unordered_map<int, T>;
		dict_<Node<ResultDataType>> ;
		dict_<Element<ResultDataType>>;
		dict_<Material> ;
		Eigen::SparseMatrix<double> stif;
		Eigen::SparseMatrix<double> mass;
};

/**
 *  Solution for response in frequency domain.
 */
template <class FileReader>
class SolutionFrequencyResponse: public SolutionBase <std::complex<double>> {
	public:
		//using SolutionBase::SolutionBase;
		SolutionFrequencyResponse(){};
		~SolutionFrequencyResponse(){};
};

/**
 * Solution for response in time domain. 
 */
class SolutionTimeResponse: public SolutionBase<double> {
	public:
		using SolutionBase::SolutionBase;
		SolutionTimeResponse(){};
		~SolutionTimeResponse(){};
};

}
#endif