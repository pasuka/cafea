/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_MESH_READER_H_
#define CAFEA_MESH_READER_H_

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <functional>
// #include <experimental/filesystem>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "fmt/format.h"

#include "cafea/fortran_wrapper.h"

namespace fs = boost::filesystem;//!< C++17 not ready yet.

namespace cafea {
/**
 *  \class FE Model Reader Basic.
 */
class FEModelContainer {
	public:
		//! Destructor.
		virtual ~FEModelContainer() {
			file_.clear();
			if (!node_list_.empty()) node_list_.clear();
			if (!elem_list_.empty()) elem_list_.clear();
		}
		//! Get node list pointer.
		const wrapper_::node_f03* get_node_ptr() const { return node_list_.data();}
		//! Get element list pointer.
		const wrapper_::elem_f03* get_element_ptr() const { return elem_list_.data();}
		//! Get statistics.
		virtual std::map<std::string, size_t> get_info() const {
			return {{"node", node_list_.size()}, {"element", elem_list_.size()}};
		}
		//! Check exists and empty of input file.
		int check_file(const std::string fn) {
			fs::path p = fn;
			if (fs::exists(p)) {
				if (!p.empty()) {
					file_ = p;
					return 0;
				} else {
					fmt::print("Input file: {} is empty!\n", fn);
					return -1;
				}
			} else {
				fmt::print("Input file: {} is not exists!\n", fn);
				return -2;
			}
		}
		//! Print.
		friend std::ostream& operator<<(std::ostream& cout, const FEModelContainer &a) {
			cout << "This is FEModelContainer.\n";
			for(const auto &p: a.get_info()) {
				cout << fmt::format("Num. of {}: {}\n", p.first, p.second);
			}
			return cout;
		}

	protected:
		fs::path file_;  //!< input file path.
		std::vector<wrapper_::node_f03> node_list_;//!< Node set.
		std::vector<wrapper_::elem_f03> elem_list_;//!< Element set.
};
/**
 *  \class BCY file reader.
 */
class BCYReader: public FEModelContainer {
	public:
		//! Destructor.
		~BCYReader() override {
			if (fp_.is_open()) fp_.close();

			if (!matl_list_.empty()) matl_list_.clear();
			if (!sect_list_.empty()) sect_list_.clear();
			if (!solu_list_.empty()) solu_list_.clear();
			if (!bc_list_.empty()) bc_list_.clear();

			if (!load_list_.empty()) {
				for (auto &x: load_list_) {
					if (!x.empty()) x.clear();
				}
				load_list_.clear();
			}
		}
		//! Load bcy file.
		int load_model(const std::string fn);
		//! Load bcy file.
		int load_model(const char* fn) {
			const std::string fn2(fn);
			return load_model(fn2);
		}
		//! Clear model data in memory.
		void clean_model();
		//! Check and print.
		std::map<std::string, size_t> get_info() const override {
			return {{"node", node_list_.size()},
				{"element", elem_list_.size()},
				{"material", matl_list_.size()},
				{"section", sect_list_.size()},
				{"boundary", bc_list_.size()},
				{"load", load_list_.size()},
				{"solution", solu_list_.size()}};
		}
		//! Get material list pointer.
		const wrapper_::matl_f03* get_material_ptr() const { return matl_list_.data();}
		//! Get section list pointer
		const wrapper_::sect_f03* get_section_ptr() const { return sect_list_.data();}
		//! Get boundary list pointer.
		const wrapper_::bndy_f03* get_boundary_ptr() const { return bc_list_.data();}
		//! Get solution list pointer.
		const wrapper_::solu_f03* get_solution_ptr() const { return solu_list_.data();}
		//! Get load list pointer.
		const std::vector<wrapper_::load_f03>* get_load_ptr() const { return load_list_.data();}
		//! Print.
		friend std::ostream& operator<<(std::ostream& cout, const BCYReader &a) {
			cout << "This is BCYReader.\n";
			for(const auto &p: a.get_info()) {
				cout << fmt::format("Num. of {}: {}\n", p.first, p.second);
			}
			return cout;
		}

	private:
		std::ifstream fp_;  //!< file handler.

		std::vector<wrapper_::matl_f03> matl_list_;
		std::vector<wrapper_::sect_f03> sect_list_;
		std::vector<wrapper_::solu_f03> solu_list_;
		std::vector<wrapper_::bndy_f03> bc_list_;
		std::vector<std::vector<wrapper_::load_f03>> load_list_;

		//! Split line by delimer.
		std::vector<std::string> parse_line(std::string line, std::string delim = ",") {
			std::vector<std::string> list;
			boost::trim(line);
			boost::split(list, line, boost::is_any_of(delim));
			return list;
		}
		//! Read node block.
		int parse_node_blk();
		//! Read element block.
		int parse_element_blk();
		//! Read material block.
		int parse_material_blk();
		//! Read section block.
		int parse_section_blk();
		//! Read boundary block.
		int parse_boundary_blk();
		//! Read solution block.
		int parse_solution_blk();
		//! Read load block.
		int parse_load_blk();
};
/**
 *  \class CDB reader.
 */
class CDBReader: public FEModelContainer {
	public:
		//! Destructor.
		~CDBReader() override {
			if (!matl_list_.empty()) matl_list_.clear();
			if (!sect_list_.empty()) sect_list_.clear();
			if (!bc_list_.empty()) bc_list_.clear();
		}
		//! Load bcy file.
		int load_model(const std::string fn);
		//! Load bcy file.
		int load_model(const char* fn) {
			const std::string fn2(fn);
			return load_model(fn2);
		}
		//! Clear all model data.
		void clean_model();
		//! Get material list pointer.
		const wrapper_::cdb_prop* get_material_ptr() const { return matl_list_.data();}
		//! Get section list pointer
		const wrapper_::cdb_prop* get_section_ptr() const { return sect_list_.data();}
		//! Get boundary list pointer.
		const wrapper_::cdb_bc* get_boundary_ptr() const { return bc_list_.data();}
		//! Check and print.
		std::map<std::string, size_t> get_info() const override {
			return {{"node", node_list_.size()},
				{"element", elem_list_.size()},
				{"material", matl_list_.size()},
				{"real_constant", sect_list_.size()},
				{"BC", bc_list_.size()}};
		}
		//! Print.
		friend std::ostream& operator<<(std::ostream& cout, const CDBReader &a) {
			cout << "This is CDBReader.\n";
			for(const auto &p: a.get_info()) {
				cout << fmt::format("Num. of {}: {}\n", p.first, p.second);
			}
			return cout;
		}

	private:
		std::vector<wrapper_::cdb_prop> matl_list_, sect_list_;
		std::vector<wrapper_::cdb_bc> bc_list_;
};
}  // namespace cafea
#endif  // CAFEA_MESH_READER_H_
