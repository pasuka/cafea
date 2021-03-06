/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_SOLUTION_HPP_
#define CAFEA_SOLUTION_HPP_

#include <cstddef>
#include <array>
#include <vector>
#include <memory>
#include <complex>
#include <ostream>
#include <typeinfo>
#include <typeindex>

#include <Eigen/Eigen>

#include "fmt/format.h"

#include "cafea/base/node.hpp"
#include "cafea/element/element.hpp"
#include "cafea/base/material.hpp"
#include "cafea/base/section.hpp"
#include "cafea/base/boundary.hpp"
#include "cafea/base/sparse_matrix.hpp"
#include "cafea/io/mesh_reader.hpp"
#include "cafea/base/eigenpair.hpp"

namespace cafea {
/**
 *  Solution base def.
 */
template <class FileReader, class Scalar = REAL4, class ResultScalar = REAL8>
class SolutionBase {
	public:
		//! Initialize environment.
		virtual void init() = 0;
		//! Clear model data.
		virtual void clear() = 0;
		//! Destructor.
		virtual ~SolutionBase() {
			fmt::print("Destructor of solution base.\n");
		}
		//! Load input file.
		virtual void load(const char* fn) {
			fmt::print("Load file:{}\n", fn);
		}
		//! Load input file.
		// virtual void load(const std::string fn) {load(fn.c_str());};
		//! Check input model data.
		virtual void check() {
			fmt::print("Check input.\n");
		}
		//! Analyze pattern.
		virtual void analyze() {
			fmt::print("Analyze model.\n");
		}
		//! Assemble global matrix.
		virtual void assembly() {
			fmt::print("Assemble matrix.\n");
		}
		//! Solve.
		virtual void solve() {
			fmt::print("Solve problem.\n");
		}
		//! Save MAT file.
		virtual void write2mat(const char* mat, bool ver_73 = false) {
			fmt::print("Save :{}\n", mat);
		}
		//! Save MAT file.
		// virtual void write2mat(const std::string mat) {write2mat(mat.c_str());};
		//! Post process.
		virtual void post_process() {
			fmt::print("Post process.\n");
		}
		//! Get model information.
		virtual std::array<size_t, 5> get_info() const {
			std::array<size_t, 5> a{0, 0, 0, 0, 0};
			return a;
		}
		//! Set mass matrix in lumped format.
		virtual void set_mass_lumped(bool t = false) {
			fmt::print("Set Lumped mass.\n");
		}
		//! Set solve option in numeric values.
		virtual void set_parameter(SolutionOption chk, init_list_<ResultScalar> val) {
			fmt::print("Set solve option. in numeric.\n");
		}
		//! Set solve option in boolean values.
		virtual void set_parameter(SolutionOption chk, bool val = false) {
			fmt::print("Set solve option in boolean.\n");
		}
		//! Get result matrix.
		virtual matrix_<ResultScalar> get_result() const {
			matrix_<ResultScalar> tmp;
			return tmp;
		}
		//! Get node result.
		virtual matrix_<ResultScalar> get_node_result(int node_id,
			LoadType res_tp, int res_span = 0) const {
			return get_result();
		}
};

/**
 *  Solution of static analysis.
 */
template <class FileReader, class Scalar = REAL4, class ResultScalar = REAL8>
class SolutionStatic: public SolutionBase <FileReader, Scalar, ResultScalar> {
	public:
		//! default constructor.
		SolutionStatic() {}
		//! Destructor.
		~SolutionStatic() override {
			fmt::print("Destructor of static analysis.\n");
			clear();
		}
		//! Initialize environment.
		void init() override;
		//! Clear model data.
		void clear() override;
		//! Load input file.
		void load(const char* fn) override;
		//! Check input model data.
		void check() override;
		//! Analyze pattern.
		void analyze() override;
		//! Assemble global matrix.
		void assembly() override;
		//! Solve.
		void solve() override;
		//! Save MAT file.
		void write2mat(const char* mat, bool ver_73 = false) override;
		//! Post process.
		void post_process() override;
		//! Get model information.
		std::array<size_t, 5> get_info()const override;
		//!
		void set_mass_lumped(bool val = false) override {
			mass_type_ = val ? MassType::LUMPED : MassType::CONSISTENT;
		}
		//!
		matrix_<ResultScalar> get_node_result(int node_id, LoadType res_tp,
			int res_span = 0) const override;

	protected:
		FileReader file_parser_;//!< Input file loader.

		dict_<Material<Scalar>> matl_group_;//!< Material dictionary.
		dict_<Section<Scalar>> sect_group_;//!< Section dictionary.
		dict_<Node<Scalar, ResultScalar>> node_group_;//!< Node dictionary.
		dict_<Element<ResultScalar>> elem_group_;//!< Element dictionary.

		std::vector<Boundary<Scalar>> bc_group_;//!< Boundary list.

		SparseMat<ResultScalar> mat_pair_;//!< Global stiffness and mass matrix.

		MassType mass_type_{MassType::CONSISTENT};

	private:
		std::vector<Load<Scalar>> load_group_;//!< Load list.
		std::unique_ptr<LinearSolver<ResultScalar>> solver_{nullptr};//!< Linear solver for Ax=b.
		SolutionType sol_type_{SolutionType::STATIC};
};

/**
 *  Solution of modal analysis.
 */
template <class FileReader, class Scalar = REAL4, class ResultScalar = REAL8>
class SolutionModal: public SolutionStatic <FileReader, Scalar, ResultScalar> {
	public:
		//! Default constructor.
		SolutionModal() {}
		//! Destructor.
		~SolutionModal() override {
			fmt::print("Destructor of modal analysis.\n");
			clear();
		}
		//! Initialize environment.
		void init() override;
		//! Clear variables.
		void clear() override;
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
		void write2mat(const char* mat, bool ver_73 = false) override;
		//! Get information.
		std::array<size_t, 5> get_info() const override;
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const SolutionModal &a) {
			return cout << "This is solution of modal analysis.\n";
		}
		//! Set solve option in numeric values.
		void set_parameter(SolutionOption chk, init_list_<ResultScalar> val) override;
		//! Set solve option in boolean values.
		void set_parameter(SolutionOption chk, bool val = false) override;
		//! Get result.
		matrix_<ResultScalar> get_result() const override { return natural_freq_;}

	protected:
		matrix_<ResultScalar> mode_shape_;//!< Mode shape of FEA model.
		matrix_<ResultScalar> natural_freq_;//!< Natural frequencies and errors.

	private:
		int freq_num_{1};
		SolutionType sol_type_{SolutionType::MODAL};
		ResultScalar freq_range_[2]={ResultScalar(0), ResultScalar(-1)};
		std::unique_ptr<EigenSolver<ResultScalar>> solver_{nullptr};//!< Eigenpair solver.
};


/**
 *  Solution of harmonic
 */
template <class FileReader, class Scalar = REAL4, class ResultScalar = REAL8>
class SolutionHarmonicFull: public SolutionStatic <FileReader, Scalar, ResultScalar> {
	public:
		//! Default constructor.
		SolutionHarmonicFull() {}
		//! Destructor.
		~SolutionHarmonicFull() override {
			fmt::print("Destructor of harmonic analysis.\n");
			clear();
		}
		//! Initialize environment.
		void init() override;
		//! Clear variables.
		void clear() override;
		//! Check input model data.
		void check() override;
		//! Load input file.
		void load(const char*) override;
		//! Analyze pattern.
		void analyze() override;
		//! Assemble global matrix.
		void assembly() override;
		//! Solve.
		void solve() override;
		//! Post process.
		void post_process() override;
		//! Save variables to MAT files.
		void write2mat(const char* mat, bool ver_73 = false) override;
		//! Get information.
		std::array<size_t, 5> get_info() const override;
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const SolutionHarmonicFull &a) {
			return cout << "This is solution of harmonic analysis.\n";
		}
		//! Set solve option in numeric values.
		void set_parameter(SolutionOption chk, init_list_<ResultScalar> val) override;
		//! Set solve option in boolean values.
		void set_parameter(SolutionOption chk, bool val = false) override;
		//! Get result.
		matrix_<ResultScalar> get_node_result(int node_id, LoadType res_tp,
			int res_span = 0) const override;

	private:
		bool has_pressure_{false};
		vecX_<ResultScalar> damping_;
		vecX_<ResultScalar> freq_range_;
		std::vector<LoadSet<Scalar>> load_group_;

		matrix_<COMPLEX<ResultScalar>> disp_cmplx_;
		matrix_<COMPLEX<ResultScalar>> rhs_cmplx_;
		SolutionType sol_type_{SolutionType::HARMONIC_FULL};
};
// //! Specialization with float type.
// template class SolutionStatic<AnsysCdbReader<REAL4>, REAL4, REAL8>;
// template class SolutionStatic<AnsysCdbReader<REAL4>, REAL4, REAL4>;
// template class SolutionModal<AnsysCdbReader<REAL4>, REAL4, REAL8>;
// template class SolutionModal<AnsysCdbReader<REAL4>, REAL4, REAL4>;
// template class SolutionHarmonicFull<AnsysCdbReader<REAL4>, REAL4, REAL8>;
// template class SolutionHarmonicFull<AnsysCdbReader<REAL4>, REAL4, REAL4>;

// template class SolutionStatic<BcyReader<REAL4>, REAL4, REAL8>;
// template class SolutionStatic<BcyReader<REAL4>, REAL4, REAL4>;
// template class SolutionModal<BcyReader<REAL4>, REAL4, REAL8>;
// template class SolutionModal<BcyReader<REAL4>, REAL4, REAL4>;
// template class SolutionHarmonicFull<BcyReader<REAL4>, REAL4, REAL8>;
// template class SolutionHarmonicFull<BcyReader<REAL4>, REAL4, REAL4>;

// template class SolutionHarmonicFull<FEModelReader<REAL4>, REAL4, REAL4>;
// template class SolutionHarmonicFull<FEModelReader<REAL4>, REAL4, REAL8>;
}  // namespace cafea
#endif  // CAFEA_SOLUTION_HPP_
