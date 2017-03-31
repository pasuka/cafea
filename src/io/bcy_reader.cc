/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include "cafea/mesh_reader.h"

namespace cafea {
/**
 * \brief Load input files.
 * \param[in] fn input file path.
 * \return 0 means okay.
 */
int BCYReader::load_model(const std::string fn) {
    fs::path p = fn;
	if (fs::exists(p)) {
		if (!p.empty()) {
			this->file_ = p;
		} else {
			fmt::print("Input file: {} is empty!\n", fn);
			return -1;
		}
	} else {
		fmt::print("Input file: {} is not exists!\n", fn);
		return -2;
	}
	if (this->fp_.is_open()) this->fp_.close();
	this->fp_.open(fn);

	if (this->fp_.is_open()) {
		std::string line;
		fmt::print("Begin to read file: {}\n", fn);
		while (std::getline(this->fp_, line)) {
			if (!line.empty()) {
				if (boost::starts_with(line, "$NODE")) {
					this->parse_node_blk();
				} else if (boost::starts_with(line, "$ELEMENT")) {
					this->parse_element_blk();
				} else if (boost::starts_with(line, "$FORMAT_INFO")) {
					//
				} else if (boost::starts_with(line, "$MATERIAL")) {
					this->parse_material_blk();
				} else if (boost::starts_with(line, "$SECTION")) {
					this->parse_section_blk();
				} else if (boost::starts_with(line, "$BOUNDARY")) {
					this->parse_boundary_blk();
				} else if (boost::starts_with(line, "$SOLUTION")) {
					this->parse_solution_blk();
				} else if (boost::starts_with(line, "$END")) {
					//
				} else if (boost::starts_with(line, "#")) {
					fmt::print("Comment line.\n");
				} else if (boost::starts_with(line, "!")) {
					fmt::print("Comment line.\n");
				} else if (boost::starts_with(line, "//")) {
					fmt::print("Comment line.\n");
				} else {
					fmt::print("Data block.\n");
				}
			}
		}
	}
	return 0;
}

/**
 * \brief Parse block of nodes.
 */
int BCYReader::parse_node_blk() {
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	int num_node = std::stoi(list[0]);
	int max_node_id = std::stoi(list[1]);
	int csys = std::stoi(list[2]);
	int has_rot = std::stoi(list[3]);
	fmt::print("Num. node: {} Max id: {} Coord. :{} xyz len: {}\n",
		num_node, max_node_id, csys, has_rot);
	for (int i = 0; i < num_node; i++) {
		std::getline(this->fp_, line);
		auto per_node = this->parse_line(line);
		wrapper_::node_f03 pt{std::stoi(per_node[0]), csys,
			{std::stof(per_node[1]), std::stof(per_node[2]), std::stof(per_node[3])}};
		if (6 == has_rot) {
			for (auto x: {0, 1, 2}) {
				pt.rot_[x] = std::stof(per_node[x+4]);
			}
		}
		this->node_list_.push_back(std::move(pt));
	}
	return 0;
}

/**
 * \brief Parse block of elements.
 */
int BCYReader::parse_element_blk() {
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	int num_elem = std::stoi(list[0]);
	int max_elem_id = std::stoi(list[1]);
	fmt::print("Num. element: {} Max id: {}\n", num_elem, max_elem_id);
	for (int i = 0; i < num_elem; i++) {
		std::getline(this->fp_, line);
		auto per_elem = this->parse_line(line);
		wrapper_::elem_f03 pp{std::stoi(per_elem[0]), std::stoi(per_elem[1]),
			std::stoi(per_elem[2]), std::stoi(per_elem[3])};
		for (int j = 0; j < std::stoi(per_elem[4]); j++) {
			pp.node_list_[j] = std::stoi(per_elem[5+j]);
		}
		this->elem_list_.push_back(std::move(pp));
	}
	return 0;
}

/**
 * \brief Parse block of materials.
 */
int BCYReader::parse_material_blk() {
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);

	int num_matl = std::stoi(list[0]);
	int max_matl_id = std::stoi(list[1]);
	fmt::print("Num. material: {} Max id: {}\n", num_matl, max_matl_id);
	for (int i = 0; i < num_matl; i++) {
		std::getline(this->fp_, line);
		auto per_matl = this->parse_line(line);
		wrapper_::matl_f03 pm{std::stoi(per_matl[0]), std::stoi(per_matl[1])};
		for (int j = 0; j < std::stoi(per_matl[2]); j++) {
			pm.val_[j] = std::stof(per_matl[3+j]);
		}
		this->matl_list_.push_back(std::move(pm));
	}
	return 0;
}

/**
 * \brief Parse block of sections.
 */
int BCYReader::parse_section_blk() {
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);

	int num_sect = std::stoi(list[0]);
	int max_sect_id = std::stoi(list[1]);
	fmt::print("Num. section: {} Max id: {}\n", num_sect, max_sect_id);
	for (int i = 0; i < num_sect; i++) {
		std::getline(this->fp_, line);
		auto per_sect = this->parse_line(line);
		wrapper_::sect_f03 ps{std::stoi(per_sect[0]), std::stoi(per_sect[1])};
		for (int j = 0; j < std::stoi(per_sect[2]); j++) {
			ps.val_[j] = std::stof(per_sect[3+j]);
		}
		this->sect_list_.push_back(std::move(ps));
	}
	return 0;
}

/**
 * \brief Parse block of boundary.
 */
int BCYReader::parse_boundary_blk() {
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);

	int num_bc = std::stoi(list[0]);
	int num_dof_per = std::stoi(list[1]);
	fmt::print("Num. boundary: {} Max dof per node: {}\n", num_bc, num_dof_per);
	for (int i = 0; i < num_bc; i++) {
		std::getline(this->fp_, line);
		auto per_bc = this->parse_line(line);
		wrapper_::bndy_f03 bc{std::stoi(per_bc[0]), std::stoi(per_bc[1])};
		for (int j = 0; j < std::stoi(per_bc[2]); j++) {
			bc.val_[j] = std::stoi(per_bc[3+j]);
		}
		this->bc_list_.push_back(std::move(bc));
	}
	return 0;
}

/**
 * \brief Parse block of loads.
 */
int BCYReader::parse_load_blk() {
	std::string line;
	std::getline(this->fp_, line);
	if (boost::starts_with(line, "$FREQ")) {
		auto list = this->parse_line(line);
		int step_id = std::stoi(list[1]);
		float freq = std::stof(list[2]);
		std::vector<wrapper_::load_f03> tmp;
		while (true) {
			std::getline(this->fp_, line);
			if (boost::starts_with(line, "$END_FREQ")) {
				break;
			} else if (boost::starts_with(line, "$NUMPRES")) {
				auto list = this->parse_line(line);
				int num_pres = std::stoi(list[1]);
				for (int i = 0; i < num_pres; i++) {
					std::getline(this->fp_, line);
					auto va = this->parse_line(line);
					wrapper_::load_f03 pres_load{std::stoi(va[1]), std::stoi(va[3]),
						-1, std::stoi(va[2]), freq, {std::stof(va[4]), std::stof(va[5])}};
					tmp.push_back(std::move(pres_load));
				}
			} else if (boost::starts_with(line, "$NUMFORCE")) {
				auto list = this->parse_line(line);
				int num_force = std::stoi(list[1]);
				for (int i = 0; i < num_force; i++) {
					std::getline(this->fp_, line);
					auto va = this->parse_line(line);
					wrapper_::load_f03 force_load{std::stoi(va[1]), 1, std::stoi(va[2]),
						-1, freq, {std::stof(va[3]), std::stof(va[4])}};
					tmp.push_back(std::move(force_load));
				}
			} else if (boost::starts_with(line, "$NUMDISP")) {
				auto list = this->parse_line(line);
				int num_disp = std::stoi(list[1]);
				for (int i = 0; i < num_disp; i++) {
					std::getline(this->fp_, line);
					auto va = this->parse_line(line);
					wrapper_::load_f03 disp_load{std::stoi(va[1]), 2, std::stoi(va[2]),
						-1, freq, {std::stof(va[3]), std::stof(va[4])}};
					tmp.push_back(std::move(disp_load));
				}
			} else {
				//
			}
		}
		assert(!tmp.empty());
		if (!tmp.empty()) this->load_list_.push_back(std::move(tmp));
	} else {
		return -1;
	}
	return 0;
}

/**
 * \brief Parse block of solution.
 */
int BCYReader::parse_solution_blk() {
	std::string line;
	std::getline(this->fp_, line);
	int num_param = std::stoi(line);

	int antype{0};
	int numfreq{0};
	float damp{0.0f};

	for (int i = 0; i < num_param; i++) {
		std::getline(this->fp_, line);
		auto list = this->parse_line(line);
		if (boost::starts_with(line, "$ANTYPE")) {
			antype = std::stoi(list[1]);
			fmt::print("Analysis type: {}\n", antype);
		} else if (boost::starts_with(line, "$DMPSTR")) {
			damp = std::stof(list[1]);
			fmt::print("Damp coeff: {}\n", damp);
		} else if (boost::starts_with(line, "$NUMFREQ")) {
			numfreq = std::stoi(list[1]);
			fmt::print("Num. Freq: {}\n", numfreq);
		} else {
			//
		}
	}
	for (int i = 0; i < numfreq; i++) {
		this->parse_load_blk();
	}

	wrapper_::solu_f03 solu{antype, numfreq, {damp, -1.0f}};
	this->solu_list_.push_back(std::move(solu));
	return 0;
}
}  // namespace cafea
