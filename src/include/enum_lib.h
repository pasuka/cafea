#ifndef ENUM_LIB_H
#define ENUM_LIB_H

namespace cafea
{
/**
 *  Coordinate system enum.
 */
enum struct CoordinateSystem {
	CARTESIAN=0,
	CYLINDRICAL,
	SPHERICAL,
	UNKNOWN=-1,
};
/**
 *  Enum of element types.
 */
enum struct ElementType {
	PIPE16, PIPE18,
	BEAM188, BEAM189, B31, B32,
	SOLID185, SOLID186, C3D4, C3D8, C3D20,
	SHELL181, SHELL281, S3R, S4R, S8R, S9R,
	MASS21, COMBIN14,
	UNKNOWN=-1,
};

/**
 *  Boundary type enum.
 */
enum struct BoundaryType {
	FIXED,
	INIT_VAL,
	UNKNOWN=-1,
};
/**
 *  DOF type enum.
 */
enum struct DofType {
	ELIMINATE=-11,
	CONSTRAINT=-12,
	PENALTY_FUN=-13,
	NORMAL=-1,
	MASTER=-2,
	SLAVE=-3,
	UNKNOWN=-99,
};
/**
 *  DOF label enum.
 */
enum struct DofLabel {
	UX=0, UY, UZ,
	URX, URY, URZ,
	WARP,
	U_ALL, UR_ALL, ALL,
	UNKNOWN,
};
/**
 *  Load type enum.
 */
enum struct LoadType {
	FORCE,
	DISP,
	VEL,
	ACCEL,
	PRES,
};
/**
 *  Load domain.
 */
enum struct LoadDomain {
	TIME,
	FREQ,
};
/**
 *  Enum of material type.
 */
enum struct MaterialType {
	LINEAR_ELASTIC,
	UNKNOWN,
};

/**
 *  Enum of material property.
 */
enum struct MaterialProp {
	DENS,//!< Mass density.
	YOUNG, EX, EY, EZ,//!< Elastic modulus.
	GXY, GYZ, GXZ,//!< Shear elastic modulus.
	NU, NUXY, NUYZ, NUXZ,//!< Minor Poisson's ratios. 
	PRXY, PRYZ, PRXZ,//!< Major Poisson's ratios.
	DMPR,//!< Constant structural damping coefficient in full harmonic analysis or damping ratio in mode-superposition analysis.
	ALPD,//!< Mass matrix multiplier for damping.
	BETD,//!< Stiffness matrix multiplier for damping.
};
/**
 *  Enum of section type.
 */
enum struct SectionType {
	PIPE, 
	SHELL,
	SOLID,
	SPRING,
	MASS,
	I_BEAM,//!< I-shaped beam section.
	L_BEAM,//!< L-shaped beam section.
	T_BEAM,//!< T-shaped beam section. 
	Z_BEAM,//!< Z-shaped beam section.
	RECT_BEAM,//!< Rectangle beam section.
	CSOLID_BEAM,//!< Circular solid beam section.
	CHAN_BEAM,//!< Channel beam section.
	HATS_BEAM,//!< Hat-shaped beam section.
	HREC_BEAM,//!< Hollow rectangle or box beam section.
	CTUBE_BEAM,//!< Circular tube section.
	UNKNOWN,
};

/**
 *  Enum of section property.
 */
enum struct SectionProp {
	OD,//!< Pipe outer diameter.
	TKWALL,//!< Wall thickness.
	RADCUR,//!< Radius of curvature.
	DENSFL,//!< Internal fluid density.
	PRESIN,//!< Internal pressure.
	ADDONMASS,//!< Add-on mass.
	ADDONSPRING,//!< Add-on spring.
};

/**
 *  Enum of solution types.
 */
enum struct SolutionType {
	STATIC,
	MODAL,
	HARMONIC_FULL,
	HARMONIC_MODAL_SUPERPOSITION,
};

/**
 *  Enum of element mass matrix form.
 */
enum struct MassType {
	LUMPED,
	CONSISTENT,
};
/**
 *
 */
enum struct SolutionOption {
	LUMPED_MASS,
	SOLVER_LDLT,
	SOLVER_SPLU,
	SOLVER_SPQR,
	MODAL_SUBSPACE,
	MODAL_LANCZOS,
	MODAL_NUMBER,
	MODAL_FREQ_RANGE,
	PRESSURE_INTERNAL,
};
}
#endif