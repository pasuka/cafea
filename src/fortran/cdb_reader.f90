module ansys
!> Experimental module to read ANSYS cdb format files.
!! Notice: Only support limited keywords.
use, intrinsic:: iso_fortran_env
use, intrinsic:: iso_c_binding
implicit none
integer, parameter:: MAX_NODES_PER_ELEM=27!> Set max. number of nodes in a elements.
integer, parameter:: LEN_ARRAY=16!> Set length of array.
!> Struct of node.
type, bind(c):: node_base
integer(c_int):: id=-1
integer(c_int):: csys=0!> Coordinate system.
!integer(c_int):: boundary(LEN_ARRAY)=0!> Boundary conditions.
!! Only supported fixed boundary.
real(c_float):: xyz(3)=0.0E0
real(c_float):: rot(3)=1.81E2!> Euler angles if needed.
end type
!> Struct of element.
type, bind(c):: elem_base
integer(c_int):: id=-1
integer(c_int):: etype=0!> Type of element.
!! Only supported No.14/16/18/21 element now.
integer(c_int):: prop_id(2)=0!> Array of material and real constant number.
integer(c_int):: opt(LEN_ARRAY)=0!> Option of element.
integer(c_int):: node_list(MAX_NODES_PER_ELEM)=-1
end type
!> Struct of material and real constant.
type, bind(c):: matl_base
integer(c_int):: id=-1
integer(c_int):: sect=0
real(c_float):: val(LEN_ARRAY)=0.0E0
end type
! Global variables.
type(node_base), target, allocatable:: model_node(:)
type(elem_base), target, allocatable:: model_elem(:)
type(matl_base), target, allocatable:: model_matl(:)
type(matl_base), target, allocatable:: model_real(:)

#ifndef PRINT_ON
#define PRINT_ON 0
#else
!> Print definition.
!! 0 means nothing.
!! 1 means print node and element.
!! 2 means output node namelist.
!! 3 means output element namelist.
!! 4 means output material and real constant namelist.
!! 5 means output node and element namelist.
!! 6 means output node element material and real constant namelist.
character(len=256):: print_fmt
logical:: group_flag(4)=[.false., .false., .false., .false.]
namelist /group_node/model_node, /group_elem/model_elem
namelist /group_matl/model_matl, model_real
namelist /group_node_elem/model_node, model_elem
namelist /group_all/model_node, model_elem, model_matl, model_real
#endif
contains
	subroutine model_init() bind(c, name='model_data_clean')
	!> @fn Clear global variables.
	implicit none
	if(allocated(model_node))deallocate(model_node)
	if(allocated(model_elem))deallocate(model_elem)
	if(allocated(model_matl))deallocate(model_matl)
	if(allocated(model_real))deallocate(model_real)
	end subroutine
	
	subroutine cdb_reader(fn, is_keep)
	!> @fn Read cdb files.
	!! @param fn file path of cdb file.
	!! @param is_keep decide cdb data will be kept in module, default is true.
	implicit none
	! Input variable.
	character(len=*), intent(in):: fn
	logical, intent(in), optional:: is_keep
	! Internal variables.
	character(len=256):: line, line_fmt, line_fmt2
	character(len=5):: keyword
	integer:: fid, fid_stat, i, j, k, ii, node_id, csys, tmp(99)
	integer:: num_node, num_elem, num_etype, num_real, num_mat, num_csys, num_secn
	integer, allocatable:: tmp_etype(:, :)
	real(kind=4):: r_xyz(6), r_tmp(LEN_ARRAY)

	! Open cdb file.
    open(newunit=fid, file=trim(fn), status='old', iostat=fid_stat)
	write(*, *)
    if(fid_stat/=0)then
    	line_fmt = '(1x, "Open file: ", a, " Failed. Error code: ", i4)'
        write(*, line_fmt)trim(fn), fid_stat
        return
    else
    	write(*, '(1x, "Begin read file: ", a)')trim(fn)
    endif
    ! initialize global variables.
    call model_init()
    ! Begin read cdb file.
    do
	    read(fid, '(A)', iostat=fid_stat)line
        ! ISO_FORTRAN_ENV define: iostat_end.
        if(fid_stat==iostat_end)exit
        if(line(1:6)=='NUMOFF')then
        	if(line(8:11)=='NODE')then
        		read(line(13:), *)num_node
        		write(*, '(1x, "Max. id of nodes:", i6)')num_node
        	elseif(line(8:11)=='ELEM')then
        		read(line(13:), *)num_elem
        		write(*, '(1x, "Max. id of elements:", i6)')num_elem
        	elseif(line(8:11)=='TYPE')then
        		read(line(13:), *)num_etype
        		write(*, '(1x, "Max. id of element types:", i3)')num_etype
        		if(allocated(tmp_etype))deallocate(tmp_etype)
	        	allocate(tmp_etype(num_etype, LEN_ARRAY+1))
    	    	tmp_etype(:, 1) = -1
    	    	tmp_etype(:, 2:) = 0
        	elseif(line(8:11)=='REAL')then
        		read(line(13:), *)num_real
        		write(*, *)'Max. id of real constants:', num_real
        	elseif(line(8:11)=='CSYS')then
        		read(line(13:), *)num_csys
        		write(*, *)'Number of coordinate systems:', num_csys
        	elseif(line(8:11)=='LINE')then
        	elseif(line(8:11)=='AREA')then
        	elseif(line(8:11)=='VOLU')then
        	elseif(line(8:11)=='SECN')then
        		read(line(13:), *)num_secn
        		write(*, *)'Number of sections:', num_secn
        	elseif(line(8:11)=='MAT ')then
        		read(line(13:), *)num_mat
        		write(*, *)'Max. id of materials:', num_mat
        		allocate(model_matl(num_mat))
        		do i = 1, num_mat
        			model_matl(i)%id = i
        		enddo
        	elseif(line(8:11)=='KP  ')then
        	elseif(line(8:11)=='CP  ')then
        	elseif(line(8:11)=='CE  ')then
        	else
        	endif
        elseif(line(1:5)=='MAT ,')then
        	read(line(6:), *)ii
        	if(ii>num_mat)then
        		write(*, '(1x, "Material number not match.", i3, "vs", i3)')ii, num_mat
        		num_mat = ii
        	endif
        elseif(line(1:5)=='REAL,')then
        	read(line(6:), *)ii
        	if(ii>num_real)num_real = ii
        elseif(line(1:5)=='TYPE,')then
        	read(line(6:), *)ii
        	if(ii>num_etype)num_etype = ii
        elseif(line(1:3)=='ET,')then
	        read(line(4:), *)i, tmp_etype(i, 1)
        elseif(line(1:5)=='KEYOP')then
        	read(line(8:), *)i, j, tmp_etype(i, j+1)
        elseif(line(1:4)=='CSYS')then
        	read(line(6:), *)csys
        elseif(line(1:7)=='RLBLOCK')then
        ! read real constants.
        	read(line(9:), *)tmp(:4)
        	if(tmp(2)/=num_real)then
        		write(*, '(1x, "Real constants not match.", i3, "vs", i3)')tmp(2), num_real
        	endif
        	num_real = tmp(1)
        	write(*, '(1x, "Number of real constants:", i4)')num_real
        	if(allocated(model_real))deallocate(model_real)
        	allocate(model_real(num_real))
        	read(fid, '(A)')line_fmt
        	read(fid, '(A)')line_fmt2
        	do i = 1, num_real
        		r_tmp = .0E0
        		read(fid, trim(line_fmt))j, k, r_tmp(:6)
        		if(k>6)read(fid, *)r_tmp(7:k)
        		model_real(i)%id = j
        		model_real(i)%val = r_tmp
        	enddo
        elseif(line(1:6)=='NBLOCK')then
        ! read node block.
            read(line(8:), *)i, keyword, j, k
            if(k>num_node)then
            	write(*, *)'Number of nodes not match:', k, 'vs', num_node
            	exit
            else
            	num_node = k
            	write(*, '(1x, "Number of nodes:", i6)')num_node
            endif
            allocate(model_node(num_node))
            read(fid, '(a)')line_fmt
            do i = 1, num_node
				r_xyz = 0.0E0
                read(fid, trim(line_fmt))node_id, j, j, r_xyz(1:6)
                model_node(i)%id = node_id
                model_node(i)%csys = csys
                model_node(i)%xyz = r_xyz(1:3)               
                if(r_xyz(4).ne.0E0.or.r_xyz(5).ne.0E0.or.r_xyz(6).ne.0E0)then            	
	                model_node(i)%rot = r_xyz(4:6)           	
	            endif
            end do
        elseif(line(1:6)=='EBLOCK')then
        ! read element block.
	        read(line(8:), *)ii, keyword, j, k
	        if(k>num_elem)then
	        	write(*, '(1x, "Number of elements not match:", i6, "vs", i6)')k, num_elem
	        else
	        	num_elem = k
	        	write(*, '(1x, "Number of elements:", i6)')num_elem
	        endif
	        allocate(model_elem(num_elem))
	        read(fid, '(A)')line_fmt
	        if(keyword=='SOLID')then	        
	        	do j = 1, num_elem
	        		read(fid, trim(line_fmt))tmp(:ii)
	        		model_elem(j)%id = tmp(11)
	        		model_elem(j)%etype = tmp_etype(tmp(2), 1)
	        		model_elem(j)%opt = tmp_etype(tmp(2), 2:)
	        		model_elem(j)%prop_id = [tmp(1), tmp(3)]
	        		tmp(99) = get_num_by_type(model_elem(j)%etype)
	        		if(tmp(99)<1)then
	        			write(*, '(1x, "Unsupported elemnt type: ", i4)')model_elem(j)%etype
	        			exit
	        		else
	        			if(tmp(99)>8)then
	        				read(fid, *)tmp(i+1:i+tmp(99)-8)
	        			endif
	        			model_elem(j)%node_list(1:tmp(99)) = tmp(12:11+tmp(99))
	        		endif	        		
	        	enddo
	        else
	        endif
        elseif(line(1:6)=='MPDATA')then
        ! read material.
        	read(line(16:19), '(a4)')keyword(1:4)
            read(line(21:), *)i, j, r_tmp(1)
            ii = maxloc(model_matl%id, 1, model_matl%id==i)
            if(ii<1)cycle
            if(keyword(1:4)=='DENS')then
            ! Mass density.
            	model_matl(ii)%val(1) = r_tmp(1)
            elseif(keyword(1:4)=='DMPR')then
            ! Constant structural damping coefficient in full harmonic analysis
            ! or damping ratio in mode-superposition analysis.
            	model_matl(ii)%val(5) = r_tmp(1)
            elseif(keyword(1:4)=='PRXY')then
            ! Major Poisson's ratios (also PRYZ, PRXZ).
            	model_matl(ii)%val(3) = r_tmp(1)
            elseif(keyword(1:4)=='PRYZ')then
            elseif(keyword(1:4)=='PRXZ')then
            elseif(keyword(1:4)=='NUXY')then
            ! Minor Poisson's ratios (also NUYZ, NUXZ).
            	model_matl(ii)%val(3) = r_tmp(1)
			elseif(keyword(1:4)=='NUYZ')then
			elseif(keyword(1:4)=='NUXZ')then
			elseif(keyword(1:4)=='GXY ')then
			! Shear moduli (also GYZ, GXZ).
				model_matl(ii)%val(4) = r_tmp(1)
			elseif(keyword(1:4)=='GYZ ')then
			elseif(keyword(1:4)=='GXZ ')then
			elseif(keyword(1:4)=='ALPD')then
			! Mass matrix multiplier for damping.
				model_matl(ii)%val(6) = r_tmp(1)
			elseif(keyword(1:4)=='BETD')then
			! Stiffness matrix multiplier for damping.
				model_matl(ii)%val(7) = r_tmp(1)
			elseif(keyword(1:4)=='ALPX')then
			! Secant coefficients of thermal expansion (also ALPY, ALPZ).
			elseif(keyword(1:4)=='ALPY')then
			elseif(keyword(1:4)=='ALPZ')then
			elseif(keyword(1:4)=='KXX ')then
			! Thermal conductivities (also KYY, KZZ).
			elseif(keyword(1:4)=='KYY ')then
			elseif(keyword(1:4)=='KZZ ')then
			elseif(keyword(1:4)=='EX  ')then
			! Elastic moduli (also EY, EZ).
				model_matl(ii)%val(2) = r_tmp(1)
			elseif(keyword(1:4)=='EY  ')then
			elseif(keyword(1:4)=='EZ  ')then
			else
                write(*,'(1x, "Unsupprot material keyword: ", A)')keyword(1:4)
            endif
        elseif(line(1:2)=='D,')then
        ! read boundary.
!	        read(line(3:), *)i, keyword, r_tmp(1:2)
!	        node_id = maxloc(model_node%id, 1, model_node%id==i)
!	        if(node_id==0)cycle
!	        if(keyword(1:2)=='UX')then
!	        	model_node(node_id)%boundary(1) = -1
!			elseif(keyword(1:2)=='UY')then
!				model_node(node_id)%boundary(2) = -1
!			elseif(keyword(1:2)=='UZ')then
!				model_node(node_id)%boundary(3) = -1
!			elseif(keyword(1:3)=='ALL')then
!				model_node(node_id)%boundary(:) = -1
!			elseif(keyword(1:4)=='ROTX')then
!				model_node(node_id)%boundary(4) = -1
!			elseif(keyword(1:4)=='ROTY')then
!				model_node(node_id)%boundary(5) = -1
!			elseif(keyword(1:4)=='ROTZ')then
!				model_node(node_id)%boundary(6) = -1
!			elseif(keyword(1:4)=='SECT')then
!			elseif(keyword(1:4)=='WARP')then
!				model_node(node_id)%boundary(7) = -1
!			else
!				write(*, '(1x, "Unsupport boundary keyword: ", A)')keyword(1:4)
!			endif
        else
        endif
    enddo
    close(fid)
    deallocate(tmp_etype)
    
#if(PRINT_ON==1)
	if(allocated(model_node))then
		if(num_node>99)write(*, '(1x, a)')'Print first 99 of nodes.'
		print_fmt = '(1x, "Node id:", i6, 1x, "XYZ:", *(F10.3))'
		do i = 1, min(99, num_node)
			associate(pt=>model_node(i))
			if(pt%rot(1)>1.8E2)then
				write(*, print_fmt)pt%id, pt%xyz
			else
				write(*, print_fmt)pt%id, pt%xyz, pt%rot
			endif
			end associate
		enddo
	endif
	if(allocated(model_elem))then
		if(num_elem>99)write(*, '(1x, "Print first 99of elements.")')
		print_fmt = '(1x, "Element id:", i4, 1x, "type:", i4, 1x, "nodes: ", *(i6))'
		do j = 1, min(99, num_elem)
			associate(pt=>model_elem(j))
			write(*, print_fmt)pt%id, pt%etype, pt%node_list(:get_num_by_type(pt%etype))
			if(sum(pt%opt)>0)write(*, '(1x, "Keyopt:", *(I2))')pt%opt
			end associate
		enddo
	endif
#endif    
#if(PRINT_ON>1)
   	open(newunit=fid, file='test_nml.txt')
   	group_flag(1:2) = [allocated(model_node), allocated(model_elem)]
   	group_flag(3:4) = [allocated(model_matl), allocated(model_real)]
#if(PRINT_ON==2)   	
   	if(group_flag(1))write(fid, nml=group_node)
#elif(PRINT_ON==3)
	if(group_flag(2))write(fid, nml=group_elem)
#elif(PRINT_ON==4)
	if(all(group_flag(3:4)))write(fid, nml=group_matl)
#elif(PRINT_ON==5)
	if(all(group_flag(1:2)))write(fid, nml=group_node_elem)
#elif(PRINT_ON==6)
	if(all(group_flag))write(fid, nml=group_all)
#else
#endif
	close(fid)
#endif
	! Clear variables.    
	if(present(is_keep))then
		if(.not.is_keep)call model_init()
	endif
	write(*, '(1x, "Finish read file.")')	
	end subroutine
	
	subroutine test_multiple_file() bind(c, name='test_multi_f')
	!> @fn Test read multiple cdb files.
	implicit none
	integer:: i, fid_stat
	character(len=256):: fn
	
	do
		read(input_unit, '(A)', iostat=fid_stat)fn
		if(fid_stat==iostat_end)exit
		i = len_trim(fn)
		if(fn(i-3:i)=='.cdb')then
			call cdb_reader(fn)
		endif
	enddo
	
	return
	end subroutine

	subroutine test_single_file(fn, str_len) bind(c, name='load_cdb_file')
	!> @fn Read single cdb file of C wrapper.
	!! @param [in] fn cdb file path.
	!! @param [in] str_len length of string.
	implicit none
	character(c_char), intent(in):: fn(*)
	integer(c_int), intent(in), value:: str_len
	
	character(len=256):: fname
	integer:: i
	do i = 1, str_len
		fname(i:i) = fn(i)
	enddo
	call cdb_reader(fname(:str_len))
	return
	end subroutine
	
	subroutine get_model_data(m_node, m_elem, m_matl, m_real, n1, n2, n3, n4) bind(c, name='model_data_ptr')
	!> @fn Get cdb file data from Fortran.
	!! @param [out] m_node pointer of node array.
	!! @param [out] m_elem pointer of element array.
	!! @param [out] m_matl pointer of material array.
	!! @param [out] m_real pointer of real constant array.
	!! @param [out] n1 number of nodes.
	!! @param [out] n2 number of elements.
	!! @param [out] n3 number of materials.
	!! @param [out] n4 number of real constants.
	implicit none
	integer(c_int), intent(out):: n1, n2, n3, n4
	type(c_ptr), intent(out)::m_node, m_elem, m_matl, m_real
	
	n1 = -1
	n2 = -1
	n3 = -1
	n4 = -1
	m_node = c_null_ptr
	m_elem = c_null_ptr
	m_matl = c_null_ptr
	m_real = c_null_ptr
	if(allocated(model_node))then
		n1 = size(model_node)
		m_node = c_loc(model_node(1))
	endif
	if(allocated(model_elem))then
		n2 = size(model_elem)
		m_elem = c_loc(model_elem(1))
	endif
	if(allocated(model_matl))then
		n3 = size(model_matl)
		m_matl = c_loc(model_matl(1))
	endif
	if(allocated(model_real))then
		n4 = size(model_real)
		m_real = c_loc(model_real(1))
	endif
	return
	end subroutine
	
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
