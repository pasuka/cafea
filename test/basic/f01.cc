#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include <string>
#include <iostream>

#include "cafea/fortran_wrapper.h"

using cafea::wrapper_::cdb_node;
using cafea::wrapper_::cdb_elem;
using cafea::wrapper_::cdb_prop;
using cafea::wrapper_::cdb_bc;
using cafea::wrapper_::cdb_reader_init;
using cafea::wrapper_::cdb_reader_info;
using cafea::wrapper_::cdb_reader_ptr;
using cafea::wrapper_::cdb_reader_load;

TEST_CASE("demo", "[CDBReader]") {
    SECTION("case 01") {
        std::string ex01("./ansys/nrc1677_1_1.cdb");
        cdb_reader_init();
        cdb_reader_load(ex01.c_str(), ex01.size());
        cdb_reader_info();
        int n1, n2, n3, n4, n5;
        cdb_node *p_node = nullptr;
        cdb_elem *p_elem = nullptr;
        cdb_prop *p_matl = nullptr, *p_rconst = nullptr;
        cdb_bc *p_bc = nullptr;
        cdb_reader_ptr(&p_node, &p_elem, &p_matl, &p_rconst, &p_bc, &n1, &n2, &n3, &n4, &n5);
        REQUIRE(11 == n1);
        REQUIRE(19 == n2);
        REQUIRE(1 == n3);
        REQUIRE(20 == n4);
        REQUIRE(11 == n5);
    }
    SECTION("case 02") {
        std::string ex("./ansys/nrc1677_1_2.cdb");
        cdb_reader_init();
        cdb_reader_load(ex.c_str(), ex.size());
        cdb_reader_info();
    }
    SECTION("multi-cases") {
        std::vector<std::string> examples{"./ansys/pipe_demo_01.cdb",
            "./ansys/pipe_demo_02.cdb", "./ansys/pipe_demo_03.cdb",
            "./ansys/nrc1677_1_1.cdb", "./ansys/nrc1677_1_2.cdb",
            "./ansys/nrc1677_1_3.cdb", "./ansys/nrc1677_1_4.cdb",
            "./ansys/nrc1677_1_5.cdb", "./ansys/nrc1677_1_6.cdb",
            "./ansys/nrc1677_1_7.cdb", "./ansys/nrc1677_2_1.cdb",
            "./ansys/nrc1677_2_2.cdb", "./ansys/nrc1677_2_3.cdb",
            "./ansys/nrc1677_2_4.cdb", "./ansys/nrc6645_1_1.cdb", };

        for (const auto fn: examples) {
            cdb_reader_init();
            cdb_reader_load(fn.c_str(), fn.size());
            cdb_reader_info();
            int n1, n2, n3, n4, n5;
            cdb_node *p_node = nullptr;
            cdb_elem *p_elem = nullptr;
            cdb_prop *p_matl = nullptr, *p_rconst = nullptr;
            cdb_bc *p_bc = nullptr;
            cdb_reader_ptr(&p_node, &p_elem, &p_matl, &p_rconst, &p_bc, &n1, &n2, &n3, &n4, &n5);
            int count_node{0};
            for (int i = 0; i < n1; i++, p_node++) {
                if (0 < p_node->id_) {
                    std::cout << "id: " << p_node->id_;
                    std::cout << " xyz: " << p_node->xyz_[0];
                    std::cout << " " << p_node->xyz_[1];
                    std::cout << " " << p_node->xyz_[2];
                    if (1.8E2 > p_node->rot_[0]) {
                        for (const auto &x: p_node->rot_) { std::cout << " " << x;}
                    }
                    std::cout << "\n";
                    count_node++;
                }
            }
            std::cout << "Total Nodes: " << count_node << "\n";
            int count_elem{0};
            for (int i = 0; i < n2; i++, p_elem++) {
                if (0 < p_elem->id_) {
                    std::cout << "id: " << p_elem->id_;
                    std::cout << " etype: " << p_elem->etype_;
                    std::cout << " material: " << p_elem->prop_[0];
                    std::cout << " real constant: " << p_elem->prop_[1] << " nodes: ";
                    for(const auto &x: p_elem->node_list_) {
                        if (0 > x) {
                            std::cout << "\n";
                            break;
                        } else {
                            std::cout << x << " ";
                        }
                    }
                    count_elem++;
                }
            }
            std::cout << "Total Elements: " << count_elem << "\n";
        }
    }
}
