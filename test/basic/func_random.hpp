#include <chrono>
#include <random>

#include "cafea/base/enum_lib.hpp"
//! Generate random component.

using cafea::LoadType;
using cafea::LoadDomain;
using cafea::DofType;
using cafea::DofLabel;
using cafea::BoundaryType;
using cafea::MaterialType;
using cafea::MaterialProp;
using cafea::SectionType;
using cafea::SectionProp;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 gen(seed);

int random_value(int a, int b) {
    std::uniform_int_distribution<> dis(a, b);
    return dis(gen);
}

template <class T = float>
T random_value(T a, T b) {
    std::uniform_real_distribution<T> dis(a, b);
    return dis(gen);
}

LoadType gen_load_type(int ith = random_value(0, 6)) {
    switch (ith) {
        case 0: return LoadType::FORCE;
        case 1: return LoadType::DISP;
        case 2: return LoadType::VEL;
        case 3: return LoadType::ACCEL;
        case 4: return LoadType::PRES;
        case 5: return LoadType::STRESS;
        case 6:
        default: return LoadType::UNKNOWN;
    }
}

LoadDomain gen_load_domain(int ith = random_value(0, 1)) {
    switch (ith) {
        case 0: return LoadDomain::TIME;
        case 1:
        default: return LoadDomain::FREQ;
    }
}

DofLabel gen_dof_label(int ith = random_value(0, 10)) {
    switch (ith) {
        case 0: return DofLabel::UX;
        case 1: return DofLabel::UY;
        case 2: return DofLabel::UZ;
        case 3: return DofLabel::URX;
        case 4: return DofLabel::URY;
        case 5: return DofLabel::URZ;
        case 6: return DofLabel::WARP;
        case 7: return DofLabel::U_ALL;
        case 8: return DofLabel::UR_ALL;
        case 9: return DofLabel::ALL;
        case 10:
        default : return DofLabel::UNKNOWN;
    }
}

BoundaryType gen_boundary_type(int ith = random_value(0, 2)) {
    switch (ith) {
        case 0: return BoundaryType::FIXED;
        case 1: return BoundaryType::INIT_VAL;
        case 2:
        default: return BoundaryType::UNKNOWN;
    }
}

MaterialType gen_material_type(int ith = random_value(0, 3)) {
    switch (ith) {
        case 0: return MaterialType::LINEAR_ELASTIC;
        case 1: return MaterialType::SPRING_STIFFNESS;
        case 2: return MaterialType::MASS_VALUES;
        case 3:
        default: return MaterialType::UNKNOWN;
    }
}

MaterialProp gen_material_prop(int ith = random_value(0, 18)) {
    switch (ith) {
        case  0: return MaterialProp::DENS;
        case  1: return MaterialProp::YOUNG;
        case  2: return MaterialProp::EX;
        case  3: return MaterialProp::EY;
        case  4: return MaterialProp::EZ;
        case  5: return MaterialProp::GXY;
        case  6: return MaterialProp::GYZ;
        case  7: return MaterialProp::GXZ;//!< Shear elastic modulus.
        case  8: return MaterialProp::NU;
        case  9: return MaterialProp::NUXY;
        case 10: return MaterialProp::NUYZ;
        case 11: return MaterialProp::NUXZ;//!< Minor Poisson's ratios.
        case 12: return MaterialProp::PRXY;
        case 13: return MaterialProp::PRYZ;
        case 14: return MaterialProp::PRXZ;//!< Major Poisson's ratios.
        case 15: return MaterialProp::DMPR;//!< Constant structural damping coefficient in full harmonic analysis or damping ratio in mode-superposition analysis.
        case 16: return MaterialProp::ALPD;//!< Mass matrix multiplier for damping.
        case 17: return MaterialProp::BETD;//!< Stiffness matrix multiplier for damping.
        case 18:
        default: return MaterialProp::UNKNOWN;
    }
}

SectionType gen_section_type(int ith = random_value(0, 15)) {
    switch (ith) {
        case  0: return SectionType::PIPE;
        case  1: return	SectionType::SHELL;
        case  2: return	SectionType::SOLID;
        case  3: return	SectionType::SPRING;
        case  4: return	SectionType::MASS;
        case  5: return	SectionType::I_BEAM;//!< I-shaped beam section.
        case  6: return	SectionType::L_BEAM;//!< L-shaped beam section.
        case  7: return	SectionType::T_BEAM;//!< T-shaped beam section.
        case  8: return	SectionType::Z_BEAM;//!< Z-shaped beam section.
        case  9: return	SectionType::RECT_BEAM;//!< Rectangle beam section.
        case 10: return	SectionType::CSOLID_BEAM;//!< Circular solid beam section.
        case 11: return	SectionType::CHAN_BEAM;//!< Channel beam section.
        case 12: return	SectionType::HATS_BEAM;//!< Hat-shaped beam section.
        case 13: return	SectionType::HREC_BEAM;//!< Hollow rectangle or box beam section.
        case 14: return	SectionType::CTUBE_BEAM;//!< Circular tube section.
        case 15:
        default: return	SectionType::UNKNOWN;
    }
}

SectionProp gen_section_prop(int ith = random_value(0, 7)) {
    switch (ith) {
        case 0: return SectionProp::OD;//!< Pipe outer diameter.
        case 1: return SectionProp::TKWALL;//!< Wall thickness.
        case 2: return SectionProp::RADCUR;//!< Radius of curvature.
        case 3: return SectionProp::DENSFL;//!< Internal fluid density.
        case 4: return SectionProp::PRESIN;//!< Internal pressure.
        case 5: return SectionProp::ADDONMASS;//!< Add-on mass.
        case 6: return SectionProp::ADDONSPRING;//!< Add-on spring.
        case 7:
        default: return SectionProp::UNKNOWN;
    }
}

DofType gen_dof_type(int ith = random_value(0, 6)) {
    switch (ith) {
        case 0: return DofType::ELIMINATE;
        case 1: return DofType::CONSTRAINT;
    	case 2: return DofType::PENALTY_FUN;
    	case 3: return DofType::NORMAL;
    	case 4: return DofType::MASTER;
    	case 5: return DofType::SLAVE;
    	case 6:
        default: return DofType::UNKNOWN;
    }
}
