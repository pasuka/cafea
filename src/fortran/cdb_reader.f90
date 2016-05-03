module ansys
use, intrinsic:: iso_fortran_env
use, intrinsic:: iso_c_binding
implicit none
integer, parameter:: MAX_NODES_PER_ELEM=27
integer, parameter:: LEN_ARRAY=10
type, bind(c):: node_base
integer(c_int):: id
integer(c_int):: csys
integer(c_int):: boundary(LEN_ARRAY)
real(c_float):: xyz(3)
real(c_float):: rot(3)
end type
type, bind(c):: elem_base
integer(c_int):: id
integer(c_int):: matl_id
integer(c_int):: etype
integer(c_int):: opt(LEN_ARRAY)
integer(c_int):: node_list(MAX_NODES_PER_ELEM)
end type
type, bind(c):: matl_base
integer(c_int):: id
integer(c_int):: sect
real(c_float):: val(2, LEN_ARRAY)
end type
! Global variables
type(node_base), allocatable:: model_node(:)
type(elem_base), allocatable:: model_elem(:)
type(matl_base), allocatable:: model_matl(:)

#ifndef PRINT_ON
#define PRINT_ON 0
#else
character(len=256):: print_fmt
logical:: group_flag(3)=[.false., .false., .false.]
namelist /group_node/model_node, /group_elem/model_elem, /group_matl/model_matl
namelist /group_node_elem/model_node, model_elem
namelist /group_all/model_node, model_elem, model_matl	
#endif


contains
	subroutine model_init()
	implicit none
	if(allocated(model_node))deallocate(model_node)
	if(allocated(model_elem))deallocate(model_elem)
	if(allocated(model_matl))deallocate(model_matl)
	end subroutine
	
	subroutine cdb_reader(fn)
	implicit none
	! Input variable
	character(len=*),intent(in):: fn
	! Internal variables
	character(len=256):: line, line_fmt
	character(len=5):: keyword
	integer:: fid, fid_stat, i, j, k, ii, node_id, csys, tmp(99)
	integer:: num_node, num_elem, num_etype, num_real, num_mat, num_csys, num_secn
	integer, allocatable:: tmp_etype(:, :)
	real(kind=4):: r_xyz(6), r_tmp(10)

	! Open cdb file
    open(newunit=fid, file=trim(fn), status='old', iostat=fid_stat)
	write(*, *)
    if(fid_stat/=0)then
    	line_fmt = '(1x, "Open file: ", a, " Failed. Error code: ", i4)'
        write(*, line_fmt)trim(fn), fid_stat
        return
    else
    	write(*, '(1x, "Begin read file: ", a)')trim(fn)
    endif
    ! initialize global variables
    call model_init()
    ! Begin read cdb file
    do
	    read(fid, '(A)', iostat=fid_stat)line
        ! ISO_FORTRAN_ENV define: iostat_end
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
        		write(*, *)'Max. id of element types:', num_etype
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
        	elseif(line(8:11)=='KP  ')then
        	elseif(line(8:11)=='CP  ')then
        	elseif(line(8:11)=='CE  ')then
        	else
        	endif
        elseif(line(1:5)=='MAT ,')then
        	read(line(6:), *)ii
        	if(ii/=num_mat)num_mat = ii
        	if(allocated(model_matl))then
        		if(size(model_matl)/=num_mat*num_real)then
        			deallocate(model_matl)
        			allocate(model_matl(num_mat*num_real))
        		endif
        	else
        		allocate(model_matl(num_mat*num_real))
        	endif
        elseif(line(1:5)=='REAL,')then
        	read(line(6:), *)ii
        	if(ii/=num_real)num_real = ii
        	if(allocated(model_matl))then
        		if(size(model_matl)/=num_mat*num_real)then
        			deallocate(model_matl)
        			allocate(model_matl(num_mat*num_real))
        		endif
        	else
        		allocate(model_matl(num_mat*num_real))
        	endif
        elseif(line(1:5)=='TYPE,')then
        	read(line(6:), *)ii
        	if(ii/=num_etype)then
        		num_etype = ii
        		deallocate(tmp_etype)
        		allocate(tmp_etype(num_etype, LEN_ARRAY+1))
        		tmp_etype(:, 1) = -1
        		tmp_etype(:, 2:) = 0
        	endif
        elseif(line(1:3)=='ET,')then
	        read(line(4:), *)i, tmp_etype(i, 1)
        elseif(line(1:5)=='KEYOP')then
        	read(line(8:), *)i, j, tmp_etype(i, j+1)
        elseif(line(1:4)=='CSYS')then
        	read(line(6:), *)csys
        elseif(line(1:7)=='RLBLOCK')then
        ! read real constants.
        	read(line(9:), *)tmp(:4)
        	if(tmp(1)>num_real)then
        	else
        		num_real = tmp(1)
        	endif
        	allocate(model_matl(num_real))
        	model_matl%id = -1
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
        	do i = 1, num_node
        		model_node(i)%id = -1
        		model_node(i)%boundary = 0
        	enddo
            read(fid, '(a)')line_fmt
            do i = 1, num_node
				r_xyz = 0.0E0
                read(fid, trim(line_fmt))node_id, j, j, r_xyz(1:6)
                model_node(i)%id = node_id
                model_node(i)%csys = csys
                model_node(i)%xyz = r_xyz(1:3)               
                if(r_xyz(4).ne.0E0.or.r_xyz(5).ne.0E0.or.r_xyz(6).ne.0E0)then            	
	                model_node(i)%rot = r_xyz(4:6)
	            else
	            	model_node(i)%rot = 1.81E2            	
	            endif
            end do
        elseif(line(1:6)=='EBLOCK')then
        ! read element block
	        read(line(8:), *)ii, keyword, j, k
	        if(k>num_elem)then
	        	write(*, '(1x, "Number of elements not match:", i6, "vs", i6)')k, num_elem
	        else
	        	num_elem = k
	        	write(*, '(1x, "Number of elements:", i6)')num_elem
	        endif
	        allocate(model_elem(num_elem))
        	model_elem%id = -1
        	model_elem%matl_id = -1
        	model_elem%etype = -1
        	do i = 1, num_elem
        		model_elem(i)%opt = 0
        		model_elem(i)%node_list = -1
        	enddo
	        read(fid, '(A)')line_fmt
	        if(keyword=='SOLID')then	        
	        	do j = 1, num_elem
	        		read(fid, trim(line_fmt))tmp(:ii)
	        		model_elem(j)%id = tmp(11)
	        		model_elem(j)%etype = tmp_etype(tmp(2), 1)
	        		model_elem(j)%opt = tmp_etype(tmp(2), 2:)
	        		model_elem(j)%matl_id = tmp(3)
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
        ! read material
        	read(line(16:19), '(a4)')keyword(1:4)
            read(line(21:), *)i, j, r_tmp(1)
            if(keyword(1:4)=='DENS')then
            ! Mass density.
            	model_matl(i)%val(1, 1) = r_tmp(1)
            elseif(keyword(1:4)=='DMPR')then
            ! Constant structural damping coefficient in full harmonic analysis
            ! or damping ratio in mode-superposition analysis.
            	model_matl(i)%val(1, 5) = r_tmp(1)
            elseif(keyword(1:4)=='PRXY')then
            ! Major Poisson's ratios (also PRYZ, PRXZ).
            	model_matl(i)%val(1, 3) = r_tmp(1)
            elseif(keyword(1:4)=='PRYZ')then
            elseif(keyword(1:4)=='PRXZ')then
            elseif(keyword(1:4)=='NUXY')then
            ! Minor Poisson's ratios (also NUYZ, NUXZ).
            	model_matl(i)%val(1, 3) = r_tmp(1)
			elseif(keyword(1:4)=='NUYZ')then
			elseif(keyword(1:4)=='NUXZ')then
			elseif(keyword(1:4)=='GXY ')then
			! Shear moduli (also GYZ, GXZ).
				model_matl(i)%val(1, 4) = r_tmp(1)
			elseif(keyword(1:4)=='GYZ ')then
			elseif(keyword(1:4)=='GXZ ')then
			elseif(keyword(1:4)=='ALPD')then
			! Mass matrix multiplier for damping.
				model_matl(i)%val(1, 6) = r_tmp(1)
			elseif(keyword(1:4)=='BETD')then
			! Stiffness matrix multiplier for damping.
				model_matl(i)%val(1, 7) = r_tmp(1)
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
				model_matl(i)%val(1, 2) = r_tmp(1)
			elseif(keyword(1:4)=='EY  ')then
			elseif(keyword(1:4)=='EZ  ')then
			else
                write(*,'(1x, "Unsupprot material keyword: ", A)')keyword(1:4)
            endif
        elseif(line(1:2)=='D,')then
        ! read boundary
	        read(line(3:), *)i, keyword, r_tmp(1:2)
	        node_id = maxloc(model_node%id, 1, model_node%id==i)
	        if(node_id==0)cycle
	        if(keyword(1:2)=='UX')then
	        	model_node(node_id)%boundary(1) = -1
			elseif(keyword(1:2)=='UY')then
				model_node(node_id)%boundary(2) = -1
			elseif(keyword(1:2)=='UZ')then
				model_node(node_id)%boundary(3) = -1
			elseif(keyword(1:3)=='ALL')then
				model_node(node_id)%boundary(:) = -1
			elseif(keyword(1:4)=='ROTX')then
				model_node(node_id)%boundary(4) = -1
			elseif(keyword(1:4)=='ROTY')then
				model_node(node_id)%boundary(5) = -1
			elseif(keyword(1:4)=='ROTZ')then
				model_node(node_id)%boundary(6) = -1
			elseif(keyword(1:4)=='SECT')then
			elseif(keyword(1:4)=='WARP')then
				model_node(node_id)%boundary(7) = -1
			else
				write(*, '(1x, "Unsupport boundary keyword: ", A)')keyword(1:4)
			endif
        else
        endif
    enddo
    close(fid)
    deallocate(tmp_etype)
    
#if(PRINT_ON==1)
	if(allocated(model_node))then
		write(*, '(1x, a)')'Print first 99 of nodes.'
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
		write(*, '(1x, "Print first 999 of elements.")')
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
   	group_flag = [allocated(model_node), allocated(model_elem), allocated(model_matl)]
#if(PRINT_ON==2)   	
   	if(group_flag(1))write(fid, nml=group_node)
#elif(PRINT_ON==3)
	if(group_flag(2))write(fid, nml=group_elem)
#elif(PRINT_ON==4)
	if(group_flag(3))write(fid, nml=group_matl)
#elif(PRINT_ON==5)
	if(all(group_flag(1:2)))write(fid, nml=group_node_elem)
#elif(PRINT_ON==6)
	if(all(group_flag))write(fid, nml=group_all)
#else
#endif
	close(fid)
#endif
	! Clear variables.    
	call model_init()
	write(*, '(1x, "Finish read file.")')	
	end subroutine
	
	integer function get_num_by_type(et)
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
