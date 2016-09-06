module header
use, intrinsic:: iso_fortran_env
use, intrinsic:: iso_c_binding
implicit none
integer, parameter:: MAX_NODES_PER_ELEM = 27!> Set max. number of nodes in a elements.
integer, parameter:: LEN_ARRAY = 16!> Set length of property array.
end module
