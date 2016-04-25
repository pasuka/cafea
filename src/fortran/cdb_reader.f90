module ansys
use, intrinsic:: iso_fortran_env
use, intrinsic:: iso_c_binding
implicit none
contains
	subroutine cdb_reader(fn)
	implicit none
	! Input variable
	character(len=*), intent(in):: fn
	! Internal variables
	character(len=256):: line, line_fmt
	character(len=5):: keyword
	integer:: fid, fid_stat, i, j, k, node_id, csys
	integer:: num_node, num_elem, num_type, num_real, num_mat, num_csys, num_secn
	real(kind=4):: r_xyz(6), r_tmp(10)
	! Open cdb file
    open(newunit=fid, file=fn, status='old', iostat=fid_stat)

    if(fid_stat/=0)then
        write(*, *)'File open failed.'
        write(*, *)'Error code is ', fid_stat
        return
    endif
    ! Begin read cdb file
    do
	    read(fid, '(A)', iostat=fid_stat)line
        ! ISO_FORTRAN_ENV define: iostat_end
        if(fid_stat==iostat_end)exit
        if(line(1:6)=='NUMOFF')then
        	if(line(8:11)=='NODE')then
        		read(line(13:), *)num_node
        		write(*, *)'Number of nodes:', num_node
        	elseif(line(8:11)=='ELEM')then
        		read(line(13:), *)num_elem
        		write(*, *)'Number of elements:', num_elem
        	elseif(line(8:11)=='TYPE')then
        		read(line(13:), *)num_type
        		write(*, *)'Number of element types:', num_type
        	elseif(line(8:11)=='REAL')then
        		read(line(13:), *)num_real
        		write(*, *)'Number of real constants:', num_real
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
        elseif(line(1:5)=='KEYOP')then
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
            do i = 1, k
				r_xyz = 0.0E0
                read(fid, trim(line_fmt))node_id, j, j, r_xyz(1:6)
            end do
        elseif(line(1:6)=='EBLOCK')then
        elseif(line(1:6)=='MPDATA')then
        elseif(line(1:2)=='D,')then
        ! read boundary
	        read(line(3:), *)i, keyword, r_tmp(1:2)
	        if(keyword(1:2)=='UX')then
			elseif(keyword(1:2)=='UY')then
			elseif(keyword(1:2)=='UZ')then
			elseif(keyword(1:3)=='ALL')then
			elseif(keyword(1:4)=='ROTX')then
			elseif(keyword(1:4)=='ROTY')then
			elseif(keyword(1:4)=='ROTZ')then
			elseif(keyword(1:4)=='SECT')then
			elseif(keyword(1:4)=='WARP')then
			else
			endif
        else
        endif
    enddo
    close(fid)
	end subroutine
end module
