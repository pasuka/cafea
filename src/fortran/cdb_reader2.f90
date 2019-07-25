!!
!! cafea --- A FEA library for dynamic analysis.
!! Copyright (c) 2007-2017 T.Q.
!! All rights reserved.
!! Distributed under GPL v3 license.
!!
module ansys
!> Experimental module to read ANSYS cdb format files.
!! Notice: Only support limited keywords.
use header
implicit none
!> Node definition.
type, bind(c):: node_base
	integer(c_int):: id = -1
	integer(c_int):: csys = 0
	real(c_float):: xyz(3) = 0.0E0
	real(c_float):: rot(3) = 1.81E2 !> Euler angles if required.
endtype
!> Element definition.
type, bind(c):: elem_base
	integer(c_int):: id = -1
	integer(c_int):: etype = 0 !> Type of element.
	integer(c_int):: prop(2) = 0 !> Material and real constant ids.
	integer(c_int):: opt(LEN_ARRAY) = 0 !> Option of element.
	integer(c_int):: node_list(MAX_NODES_PER_ELEM) = -1
endtype
!> Material definition.
type, bind(c):: prop_base
	integer(c_int):: id = -1
	integer(c_int):: prop_type = -1
	real(c_float):: arrb(LEN_ARRAY) = 0.0E0
	real(c_float):: arrb2(LEN_ARRAY) = .0E0
endtype
!> Boundary definition.
type, bind(c):: bc_base
	integer(c_int):: id = -1
	integer(c_int):: dof_label(LEN_ARRAY) = -1
	real(c_float):: val(LEN_ARRAY) = 0.0E0
	real(c_float):: val_im(LEN_ARRAY) = 0.0E0
endtype
!> Element type option.
type, bind(c):: elem_type
	integer(c_int):: etype = -1
	integer(c_int):: opt(LEN_ARRAY) = 0
endtype
! Global variables.
type(node_base), target, allocatable:: model_node(:)
type(elem_base), target, allocatable:: model_elem(:)
type(prop_base), target, allocatable:: model_matl(:)
type(prop_base), target, allocatable:: model_const(:)
type(bc_base), target, allocatable:: model_bc(:)
type(elem_type), allocatable:: model_etype(:)
real(c_float), parameter:: ZERO = 0.0E0
!!
#ifndef PRINT_LEVEL
#define PRINT_LEVEL 0
#endif
contains
	subroutine model_init() bind(c, name = "cdb_reader_init")
	!> @fn Clear global variables.
	implicit none
	if (allocated(model_node)) deallocate(model_node)
	if (allocated(model_elem)) deallocate(model_elem)
	if (allocated(model_matl)) deallocate(model_matl)
	if (allocated(model_const)) deallocate(model_const)
	if (allocated(model_bc)) deallocate(model_bc)
	if (allocated(model_etype)) deallocate(model_etype)
#if (1 <= PRINT_LEVEL)
	write(*, "(A)")repeat("*", 80)
	write(*, "('CDB model container cleared!')")
	write(*, "(A)")repeat(" ", 80)
#endif
	end subroutine

	subroutine model_prnt() bind(c, name = "cdb_reader_info")
	!> @fn Print model statistics.
	implicit none
	character(len = :), allocatable:: line_fmt
	if (allocated(model_node)) then
		line_fmt = "('         Node max id:', 1X, I6, 1X, 'Total:', 1X, I6)"
		write(*, line_fmt)size(model_node), count(model_node%id .GT. 0)
		deallocate(line_fmt)
	else
		write(*, '("Node array is empty!")')
	endif
	if (allocated(model_elem)) then
		line_fmt = "('      Element max id:', 1X, I6, 1X, 'Total:', 1X, I6)"
		write(*, line_fmt)size(model_elem), count(model_elem%id .GT. 0)
		deallocate(line_fmt)
	else
		write(*, '("Element array is empty!")')
	endif
	if (allocated(model_matl)) then
		line_fmt = "('     Material max id:', 1X, I6, 1X, 'Total:', 1X, I6)"
		write(*, line_fmt)size(model_matl), count(model_matl%id .GT. 0)
		deallocate(line_fmt)
	else
		write(*, '("Material array is empty!")')
	endif
	if (allocated(model_const)) then
		line_fmt = "('Real constant max id:', 1X, I6, 1X, 'Total:', 1X, I6)"
		write(*, line_fmt)size(model_const), count(model_const%id .GT. 0)
		deallocate(line_fmt)
	else
		write(*, '("Real constant is empty!")')
	endif
	end subroutine

	subroutine model_get_ptr(node, elem, prop, prop2, bc, n1, n2, n3, n4, n5) bind(c, name = "cdb_reader_ptr")
	!> @fn Get model pointer.
	implicit none
	type(c_ptr), intent(out):: node, elem, prop, prop2, bc
	integer(c_int), intent(out):: n1, n2, n3, n4, n5
	n1 = -1; n2 = -1; n3 = -1; n4 = -1; n5 = -1
	node = c_null_ptr; elem = c_null_ptr; prop = c_null_ptr
	prop2 = c_null_ptr; bc = c_null_ptr

	if (allocated(model_node)) then
		n1 = size(model_node)
		node = c_loc(model_node(1))
	endif
	if (allocated(model_elem)) then
		n2 = size(model_elem)
		elem = c_loc(model_elem(1))
	endif
	if (allocated(model_matl)) then
		n3 = size(model_matl)
		prop = c_loc(model_matl(1))
	endif
	if (allocated(model_const)) then
		n4 = size(model_const)
		prop2 = c_loc(model_const(1))
	endif
	if (allocated(model_bc)) then
		n5 = size(model_bc)
		bc = c_loc(model_bc(1))
	endif
	end subroutine

	subroutine model_load(fn)
	!> @fn Load CDB file.
	!! @param fn file path.
	implicit none
	character(len = *), intent(in):: fn
	character(len = 256):: line, line_fmt, line_fmt2
	character(len = 5):: keyword
	integer:: fid, fid_stat, i, j, k, ii, jj, kk, n1, n2, n3, nvec(19)
	real(c_float):: val_im, val_re, rot(3)

	! Open cdb file.
	open(newunit = fid, file = trim(fn), status = 'old', iostat = fid_stat)
	if (fid_stat /= 0) then
		write(*, '("Open:", A, 1X, "Failed!")')trim(fn)
		write(*, '("Error code:", I8)')fid_stat
		return
	else
		write(*, "(A)")repeat("=", 80)
		write(*, '("Begin read:", A)')trim(fn)
		write(*, "(A)")repeat(" ", 80)
	endif
	call model_init()
	do
		read(fid, '(A)', iostat = fid_stat)line
		! ISO_FORTRAN_ENV define: iostat_end.
		if (fid_stat == iostat_end) exit
		if (line(:7) == "NUMOFF,") then
			if (line(8:12) == "NODE,") then
				read(line(13:), *)ii
				allocate(model_node(ii))
				allocate(model_bc(ii))
			elseif (line(8:12) == "ELEM,") then
				read(line(13:), *)jj
				allocate(model_elem(jj))
			elseif (line(8:12) == "MAT ,") then
				read(line(13:), *)kk
				allocate(model_matl(kk))
			elseif (line(8:12) == "REAL,") then
				read(line(13:), *)i
				allocate(model_const(i))
			elseif (line(8:12) == "TYPE,") then
				read(line(13:), *)j
				allocate(model_etype(j))
			else
#if (2 <= PRINT_LEVEL)
				write(*, '("Ignored:", 1X, A)')line(8:)
#endif
			endif
		elseif (line(:8) == "RLBLOCK,") then
			read(fid, '(A)')line_fmt
			read(fid, '(A)')line_fmt2
			read(line(9:), *)k, n1, n2, n3
			do ii = 1, k
				read(fid, trim(line_fmt))i, j, model_const(i)%arrb(:6)
				model_const(i)%id = i
				if (6 < j .AND. j <= LEN_ARRAY) then
					read(fid, trim(line_fmt2))model_const(i)%arrb(7:13)
				endif
			enddo
		elseif (line(:7) == "NBLOCK,") then
			read(line(8:), *)i, keyword, j, k
			read(fid, '(A)')line_fmt
			do ii = 1, k
				rot = ZERO
				read(fid, trim(line_fmt))jj, n2, n3, model_node(jj)%xyz, rot
				model_node(jj)%id = jj
				if (any(rot .NE. ZERO)) model_node(jj)%rot = rot
			enddo
		elseif (line(:7) == "EBLOCK,") then
			read(line(8:), *)i, keyword, j, k
			read(fid, '(A)')line_fmt
			do ii = 1, k
				read(fid, trim(line_fmt))nvec
				model_elem(nvec(11))%id = nvec(11)
				model_elem(nvec(11))%prop = nvec([1, 3])
				model_elem(nvec(11))%etype = model_etype(nvec(2))%etype
				model_elem(nvec(11))%opt = model_etype(nvec(2))%opt
				n1 = get_num_by_type(model_etype(nvec(2))%etype)
				if (1 <= n1 .and. n1 <= 8) then
					model_elem(nvec(11))%node_list(:n1) = nvec(12:11+n1)
				elseif (9 <= n1 .and. n1 <= MAX_NODES_PER_ELEM) then
					model_elem(nvec(11))%node_list(:8) = nvec(12:)
					read(fid, trim(line_fmt))model_elem(nvec(11))%node_list(9:n1)
				else
				endif
			enddo
		elseif (line(:6) == "KEYOP,") then
			read(line(7:), *)ii, kk, model_etype(ii)%opt(kk)
		elseif (line(:3) == "ET,") then
			read(line(4:), *)ii, model_etype(ii)%etype
		elseif (line(:2) == "D,") then
			read(line(3:), *)ii, keyword, val_re, val_im
			kk = get_index_by_dof_label(keyword(:4))
			if (0 < kk .and. kk < 999) then
				model_bc(ii)%id = ii
				model_bc(ii)%dof_label(kk) = 1
				model_bc(ii)%val(kk) = val_re
				model_bc(ii)%val_im(kk) = val_im
			elseif (999 == kk) then
				model_bc(ii)%id = ii
				model_bc(ii)%dof_label = 1
				model_bc(ii)%val = val_re
				model_bc(ii)%val_im = val_im
			else
			endif
		elseif (line(:12) == "MPDATA,R5.0,") then
			read(line(13:), *)i, keyword(:4), j, k, rot(1)!, rot(2), rot(3)
			if (j .NE. model_matl(j)%id) model_matl(j)%id =j
			kk = get_index_by_material_label(keyword(:4))
			if (0 < kk) then
				model_matl(j)%arrb(kk) = rot(1)
			endif
		else
#if (2 <= PRINT_LEVEL)
			write(*, "('Ignored:', 1X, A)")trim(line)
#endif
		endif
	end do
	close(fid)
#if (1 <= PRINT_LEVEL)
	write(*, "(A)")repeat("+", 80)
#endif
	end subroutine

	subroutine test_single_file(fn, str_len) bind(c, name = "cdb_reader_load")
	!> @fn Read single cdb file of C wrapper.
	!! @param [in] fn cdb file path.
	!! @param [in] str_len length of string.
	implicit none
	character(c_char), intent(in):: fn(*)
	integer(c_int), intent(in), value:: str_len

	character(len = 256):: fname
	integer:: i
	do i = 1, str_len
		fname(i:i) = fn(i)
	enddo
	call model_load(fname(:str_len))
	return
	end subroutine

	integer function get_index_by_material_label(kw)
	!> Get index of material label.
	!! @param [in] kw material label.
	!! @param [out] index of material label.
	implicit none
	character(len = 4), intent(in):: kw

	if (kw == "DENS") then
		get_index_by_material_label = 1
	elseif (kw == "NUXY") then
		get_index_by_material_label = 3
	elseif (kw(:2) == "EX") then
		get_index_by_material_label = 2
	elseif (kw(:3) == "GXY") then
		get_index_by_material_label = 4
	elseif (kw(:3) == "KXX") then
		get_index_by_material_label = -1
	elseif (kw == "ALPX") then
		get_index_by_material_label = -1
	else
		get_index_by_material_label = -1
	endif
	return
	end function

	integer function get_index_by_dof_label(kw)
	!> Get index of DOF label.
	!! @param [in] kw dof label.
	!! @param [out] index of dof.
	implicit none
	character(len = 4), intent(in):: kw

	if (kw(:2) == "UX") then
		get_index_by_dof_label = 1
	elseif (kw(:2) == "UY") then
		get_index_by_dof_label = 2
	elseif (kw(:2) == "UZ") then
		get_index_by_dof_label = 3
	elseif (kw == "ROTX") then
	 	get_index_by_dof_label = 4
	elseif (kw == "ROTY") then
		get_index_by_dof_label = 5
	elseif (kw == "ROTZ") then
		get_index_by_dof_label = 6
	elseif (kw == "WARP") then
		get_index_by_dof_label = 7
	elseif (kw(:3) == "ALL") then
		get_index_by_dof_label = 999
	elseif (kw == "SECT") then
		get_index_by_dof_label = 999
	else
		get_index_by_dof_label = -1
	endif
	return
	end function

	integer function get_num_by_type(et)
	!> Get number of nodes by element type.
	!! @param [in] et element type.
	!! @param [out] number of nodes.
	implicit none
	integer, intent(in):: et

	select case(et)
		case(21)
			get_num_by_type = 1
			return
		case(188, 16, 14, 288)
			get_num_by_type = 2
			return
		case(18, 189, 289, 290)
			get_num_by_type = 3
			return
		case(63, 181)
			get_num_by_type = 4
			return
		case(185, 281)
			get_num_by_type = 8
			return
		case(186)
			get_num_by_type = 20
			return
		case default
			get_num_by_type = -1
			return
		end select
	end function
end module
