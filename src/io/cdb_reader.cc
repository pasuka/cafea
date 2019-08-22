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
int CDBReader::load_model(const std::string fn) {
    int flag = this->check_file(fn);
    if (0 > flag) return flag;

    wrapper_::cdb_reader_init();
    wrapper_::cdb_reader_load(fn.c_str(), fn.size());

    int n1, n2, n3, n4, n5;
    wrapper_::cdb_node *p_node = nullptr;
    wrapper_::cdb_elem *p_elem = nullptr;
    wrapper_::cdb_prop *p_matl = nullptr, *p_rconst = nullptr;
    wrapper_::cdb_bc *p_bc = nullptr;
    cdb_reader_ptr(&p_node, &p_elem, &p_matl, &p_rconst, &p_bc, &n1, &n2, &n3, &n4, &n5);
    for (int i = 0; i < n1; i++, p_node++) {
        if (0 < p_node->id_) this->node_list_.push_back(*p_node);
    }
    for (int i = 0; i < n2; i++, p_elem++) {
        if (0 < p_elem->id_) this->elem_list_.push_back(*p_elem);
    }
    for (int i = 0; i < n3; i++, p_matl++) {
        if (0 < p_matl->id_) this->matl_list_.push_back(*p_matl);
    }
    for (int i = 0; i < n4; i++, p_rconst++) {
        if (0 < p_rconst->id_) this->sect_list_.push_back(*p_rconst);
    }
    for (int i = 0; i < n5; i++, p_bc++) {
        if (0 < p_bc->id_) {
            if (std::any_of(std::begin(p_bc->dof_label_), std::end(p_bc->dof_label_), [] (int x) { return 0 < x;})) {
                this->bc_list_.push_back(*p_bc);
            }
        }
    }
    wrapper_::cdb_reader_init();
    return 0;
}
/**
 *  \brief Clean model data.
 */
void CDBReader::clean_model() {
    this->file_.clear();
    if (!this->node_list_.empty()) this->node_list_.clear();
    if (!this->elem_list_.empty()) this->elem_list_.clear();
    if (!this->matl_list_.empty()) this->matl_list_.clear();
    if (!this->sect_list_.empty()) this->sect_list_.clear();
    if (!this->bc_list_.empty()) this->bc_list_.clear();
}
}  // namespace cafea
