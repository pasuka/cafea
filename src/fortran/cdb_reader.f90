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
contains
	subroutine model_init() bind(c, name='model_init')
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
	integer:: fid, fid_stat, i, j, k, node_id, csys
	integer:: num_node, num_elem, num_etype, num_real, num_mat, num_csys, num_secn
	integer, allocatable:: tmp_etype(:, :)
	real(kind=4):: r_xyz(6), r_tmp(10)
#ifndef PRINT_ON
#define PRINT_ON 0
#else
	character(len=256):: print_fmt
#endif	
	! Open cdb file
    open(newunit=fid, file=trim(fn), status='old', iostat=fid_stat)

    if(fid_stat/=0)then
        write(*, *)'File open failed.'
        write(*, *)'Error code is ', fid_stat
        return
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
        		write(*, *)'Number of nodes:', num_node
        		allocate(model_node(num_node))
        		do i = 1, num_node
        		    model_node(i)%id = -1
        			model_node(i)%boundary = 0
        		enddo
        	elseif(line(8:11)=='ELEM')then
        		read(line(13:), *)num_elem
        		write(*, *)'Number of elements:', num_elem
        		allocate(model_elem(num_elem))
        		model_elem%id = -1
        	elseif(line(8:11)=='TYPE')then
        		read(line(13:), *)num_etype
        		write(*, *)'Number of element types:', num_etype
        		if(allocated(tmp_etype))deallocate(tmp_etype)
        		allocate(tmp_etype(num_etype, LEN_ARRAY))
        		tmp_etype = -1
        	elseif(line(8:11)=='REAL')then
        		read(line(13:), *)num_real
        		write(*, *)'Number of real constants:', num_real
        		allocate(model_matl(num_real))
        		model_matl%id = -1
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
        		write(*, *)'Number of materials:', num_mat
        	elseif(line(8:11)=='KP  ')then
        	elseif(line(8:11)=='CP  ')then
        	elseif(line(8:11)=='CE  ')then
        	else
        	endif
        elseif(line(1:3)=='ET,')then
	        read(line(4:), *)i, tmp_etype(i, 1)
        elseif(line(1:5)=='KEYOP')then
        	read(line(8:), *)i, j, tmp_etype(i, j+1)
        elseif(line(1:4)=='CSYS')then
        	read(line(6:), *)csys
        elseif(line(1:7)=='RLBLOCK')then
        elseif(line(1:6)=='NBLOCK')then
        ! read node block
            read(line(8:), *)i, keyword, j, k
            if(k>num_node)then
            	write(*, *)'Number of nodes not match:', k, 'vs', num_node
            	exit
            endif
            read(fid, '(a)')line_fmt
#if(PRINT_ON==1)
			print_fmt = '(1x, "Node id:", i6, 1x, "XYZ:", *(F12.5))'
#endif            
            do i = 1, k
				r_xyz = 0.0E0
                read(fid, trim(line_fmt))node_id, j, j, r_xyz(1:6)
                model_node(i)%id = node_id
                model_node(i)%csys = csys
                model_node(i)%xyz = r_xyz(1:3)               
                if(r_xyz(4).ne.0E0.or.r_xyz(5).ne.0E0.or.r_xyz(6).ne.0E0)then            	
	                model_node(i)%rot = r_xyz(4:6)
#if(PRINT_ON==1)	
					write(*, print_fmt)node_id, r_xyz(1:6)
#endif  
	            else
	            	model_node(i)%rot = 1.81E2
#if(PRINT_ON==1)
					write(*, print_fmt)node_id, r_xyz(1:3)
#endif 	            	
	            endif
            end do
        elseif(line(1:6)=='EBLOCK')then
        elseif(line(1:6)=='MPDATA')then
        elseif(line(1:2)=='D,')then
        ! read boundary
	        read(line(3:), *)i, keyword, r_tmp(1:2)
	        node_id = maxloc(model_node(:)%id, 1, model_node(:)%id==i)
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
			endif
        else
        endif
    enddo
    close(fid)
	end subroutine
end module
