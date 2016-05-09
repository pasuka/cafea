program cdb_test
use:: ansys
implicit none
character(len=256):: fn
integer:: arg_cnt, i, j, fid_stat

arg_cnt = command_argument_count()
write(*, '(1x, A)')'*************************************'
write(*, '(1x, "Input argument count:", i4)')arg_cnt
write(*, '(1x, A)')'*************************************'
if(arg_cnt>0)then
	do i = 1, arg_cnt
		call get_command_argument(i, fn)
		j = len_trim(fn) 
		if(fn(j-3:j)=='.cdb')then
			call cdb_reader(fn)
		endif
	enddo
endif
! Read from stdin
do
	read(input_unit, '(A)', iostat=fid_stat)fn
	if(fid_stat==iostat_end)exit
	i = len_trim(fn)
	if(fn(i-3:i)=='.cdb')then
		call cdb_reader(fn)
	endif
enddo
end program
