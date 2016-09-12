module bcy
use header
implicit none
!> Struct of node.
type, bind(c):: node_bcy
integer(c_int):: id = -1
integer(c_int):: csys = 0!> Coordinate system.
real(c_float):: xyz(3) = .0E0!> XYZ values.
real(c_float):: rot(3) = 1.81E2!> Euler angles.
end type
!> Struct of element.
type, bind(c):: elem_bcy
integer(c_int):: id = -1
integer(c_int):: etype = -1!> ID of element type.
integer(c_int):: mtype = -1!> ID of material type.
integer(c_int):: stype = -1!> ID of section type.
integer(c_int):: node_list(MAX_NODES_PER_ELEM) = -1!> List of node id.
end type
!> Struct of material.
type, bind(c):: matp_bcy
integer(c_int):: id = -1
integer(c_int):: mt_type = -1
real(c_float):: val(LEN_ARRAY) = .0E0
end type
!> Struct of section.
type, bind(c):: sect_bcy
integer(c_int):: id = -1
integer(c_int):: st_type = -1
real(c_float):: val(LEN_ARRAY) = .0E0
end type
!> Struct of boundary.
type, bind(c):: bndy_bcy
integer(c_int):: id = -1
integer(c_int):: bc_type = -1
real(c_float):: val(LEN_ARRAY) = .0E0
end type
!> Struct of load.
type, bind(c):: load_bcy
integer(c_int):: id = -1
integer(c_int):: load_type = -1
real(c_float):: val(LEN_ARRAY) = .0E0
end type
! Global variables.
type(node_bcy), target, allocatable:: model_node(:)
type(elem_bcy), target, allocatable:: model_elem(:)
type(matp_bcy), target, allocatable:: model_matl(:)
type(sect_bcy), target, allocatable:: model_sect(:)
type(load_bcy), target, allocatable:: model_load(:)
type(bndy_bcy), target, allocatable:: model_bndy(:)
contains
    subroutine model_init() bind(c, name='model_data_clean_bcy')
	!> @fn Clear global variables.
	implicit none
	if(allocated(model_node))deallocate(model_node)
	if(allocated(model_elem))deallocate(model_elem)
	if(allocated(model_matl))deallocate(model_matl)
	if(allocated(model_sect))deallocate(model_sect)
    if(allocated(model_load))deallocate(model_load)
    if(allocated(model_bndy))deallocate(model_bndy)
	end subroutine

    subroutine bcy_reader(fn, is_keep)
	!> @fn Read bcy files.
	!! @param fn file path of bcy file.
	!! @param is_keep decide bcy data will be kept in module, default is true.
	implicit none
	! Input variable.
	character(len=*), intent(in):: fn
	logical, intent(in), optional:: is_keep
    ! Internal variables.
	character(len=256):: line, line_data, line_fmt, line_fmt2
	character(len=5):: keyword
    integer:: fid, fid_stat, i, j, k
    integer:: num_node, max_node, csys
    integer:: num_elem, max_elem
    integer:: num_matl, max_matl
    integer:: num_sect, max_sect
    integer:: num_load, max_load
    integer:: num_bndy, max_bndy
    real(kind=4):: r1, r2

    ! Open cdb file.
    open(newunit=fid, file=trim(fn), status='old', iostat=fid_stat)
    if(fid_stat/=0)then
    	line_fmt = '(1x, "Open file: ", a, " Failed. Error code: ", i4)'
        write(*, line_fmt)trim(fn), fid_stat
        return
    else
    endif
    ! initialize global variables.
    call model_init()
    ! Begin read cdb file.
    do
	    read(fid, '(A)', iostat=fid_stat)line
        ! ISO_FORTRAN_ENV define: iostat_end.
        if(fid_stat==iostat_end)exit
        line = adjustl(trim(line))
        if(line(1:1)=='$')then
            if(line(2:5)=='END_')then
                cycle
            elseif(line(2:5)=='NODE')then
                read(fid, *)num_node, max_node, csys, k
                allocate(model_node(num_node))
                do i = 1, num_node
                    model_node(i)%csys = csys
                    if(k==6)then
                        read(fid, *)model_node(i)%id, model_node(i)%xyz(1:3), model_node(i)%rot(1:3)
                    else
                        read(fid, *)model_node(i)%id, model_node(i)%xyz(1:3)
                    endif
                enddo
            elseif(line(2:5)=='ELEM')then
                read(fid, *)num_elem, max_elem
                allocate(model_elem(num_elem))
                do i = 1, num_elem
                    associate(p=>model_elem(i))
                    read(fid, *)p%id, p%etype, p%mtype, p%stype, j, p%node_list(1:j)
                    end associate
                enddo
            elseif(line(2:5)=='MATL')then
                read(fid, *)num_matl, max_matl
                allocate(model_matl(num_matl))
                do i = 1, num_matl
                    associate(p=>model_matl(i))
                    read(fid, *)p%id, p%mt_type, j, p%val(1:j)
                    end associate
                enddo
            elseif(line(2:5)=='SECT')then
                read(fid, *)num_sect, max_sect
                allocate(model_sect(num_sect))
                do i = 1, num_sect
                    associate(p=>model_sect(i))
                    read(fid, *)p%id, p%st_type, j, p%val(1:j)
                    end associate
                enddo
            elseif(line(2:5)=='LOAD')then
                read(fid, *)num_load, max_load
                if(num_load<1)then
                    cycle
                else
                    allocate(model_load(num_load))
                    do i = 1, num_load
                        associate(p=>model_load(i))
                        read(fid, *)p%id, p%load_type, j, p%val(1:j)
                        end associate
                    enddo
                endif
            elseif(line(2:5)=='BNDY')then
                read(fid, *)num_bndy, max_bndy
                if(num_bndy<1)then
                    cycle
                else
                    allocate(model_bndy(num_bndy))
                    do i = 1, num_bndy
                        associate(p=>model_bndy(i))
                        read(fid, *)p%id, p%bc_type, j, p%val(1:j)
                        end associate
                    enddo
                endif
            else
            endif
        elseif(line(1:1)=='!')then
            write(*, '("This is a comment line.")')
            cycle
        else
            write(*, '("Unsupported keyword:", A)')line(1:1)
        endif
    enddo
    close(fid)

    ! Clear variables.
	if(present(is_keep))then
		if(.not.is_keep)call model_init()
	endif
    return
    end subroutine

    subroutine load_single_file(fn, str_len) bind(c, name='load_bcy_file')
	!> @fn Read single bcy file of C wrapper.
	!! @param [in] fn bcy file path.
	!! @param [in] str_len length of string.
	implicit none
	character(c_char), intent(in):: fn(*)
	integer(c_int), intent(in), value:: str_len

	character(len=256):: fname
	integer:: i
	do i = 1, str_len
		fname(i:i) = fn(i)
	enddo
	call bcy_reader(fname(:str_len))
	return
	end subroutine

    subroutine get_model_data(m_node, m_elem, m_matl, m_sect, m_bndy, m_load,&
    &n1, n2, n3, n4, n5, n6) bind(c, name='model_data_ptr_bcy')
	!> @fn Get bcy file data from Fortran.
	!! @param [out] m_node pointer of node array.
	!! @param [out] m_elem pointer of element array.
	!! @param [out] m_matl pointer of material array.
	!! @param [out] m_sect pointer of section array.
    !! @param [out] m_bndy pointer of boundary array.
    !! @param [out] m_load pointer of load array.
	!! @param [out] n1 number of nodes.
	!! @param [out] n2 number of elements.
	!! @param [out] n3 number of materials.
	!! @param [out] n4 number of sections.
    !! @param [out] n5 number of boundaries.
    !! @param [out] n6 number of loads.
	implicit none
	integer(c_int), intent(out):: n1, n2, n3, n4, n5, n6
	type(c_ptr), intent(out)::m_node, m_elem, m_matl, m_sect, m_bndy, m_load

	n1 = -1
	n2 = -1
	n3 = -1
	n4 = -1
    n5 = -1
    n6 = -1

	m_node = c_null_ptr
	m_elem = c_null_ptr
	m_matl = c_null_ptr
	m_sect = c_null_ptr
    m_bndy = c_null_ptr
    m_load = c_null_ptr

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

    if(allocated(model_sect))then
		n4 = size(model_sect)
		m_sect = c_loc(model_sect(1))
	endif

    if(allocated(model_bndy))then
        n5 = size(model_bndy)
        m_bndy = c_loc(model_bndy(1))
    endif

    if(allocated(model_load))then
        n6 = size(model_load)
        m_load = c_loc(model_load(1))
    endif
	return
	end subroutine
end module
