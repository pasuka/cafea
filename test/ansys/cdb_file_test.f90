program cdb_test
use:: ansys
implicit none
character(len=256):: fn
integer:: arg_cnt

arg_cnt = command_argument_count()
write(*, '(1x, A)')'*************************************'
write(*, '(1x, "Input argument count:", i4)')arg_cnt
write(*, '(1x, A)')'*************************************'
if(arg_cnt>0)then
	call get_command_argument(1, fn)
	call cdb_reader(fn)
endif
!call execute_command_line('dir *.exe')
end program
