!!
!! cafea --- A FEA library for dynamic analysis.
!! Copyright (c) 2007-2017 T.Q.
!! All rights reserved.
!! Distributed under GPL v3 license.
!!
module header
use, intrinsic:: iso_fortran_env
use, intrinsic:: iso_c_binding
implicit none
integer, parameter:: MAX_NODES_PER_ELEM = 27!> Set max. number of nodes in a elements.
integer, parameter:: LEN_ARRAY = 16!> Set length of property array.
end module
