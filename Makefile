
#This is a hack to pass arguments to the run command and probably only 
#works with gnu make. 
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

CC = gcc
CFLAGS = -g  -Werror-implicit-function-declaration

SRC = fatinfo.c direntry.c directory.c disk.c fat12.c fat32.c 
OBJ = $(SRC:.c=.o)

fatinfo: $(OBJ)

depend: $(SRC)
	makedepend $(SRC)

clean:
	/bin/rm -f $(OBJ) fatinfo *~

.PHONY: run
run: fatinfo  
	./fatinfo $(RUN_ARGS)

# DO NOT DELETE

fatinfo.o: fatinfo.h /usr/include/stdlib.h /usr/include/features.h
fatinfo.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
fatinfo.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
fatinfo.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
fatinfo.o: /usr/include/endian.h /usr/include/bits/endian.h
fatinfo.o: /usr/include/bits/byteswap.h /usr/include/bits/types.h
fatinfo.o: /usr/include/bits/typesizes.h /usr/include/bits/byteswap-16.h
fatinfo.o: /usr/include/sys/types.h /usr/include/time.h
fatinfo.o: /usr/include/sys/select.h /usr/include/bits/select.h
fatinfo.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
fatinfo.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
fatinfo.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h fat12.h
fatinfo.o: fat32.h /usr/include/stdio.h /usr/include/libio.h
fatinfo.o: /usr/include/_G_config.h /usr/include/wchar.h
fatinfo.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fatinfo.o: directory.h direntry.h disk.h /usr/include/stdint.h
fatinfo.o: /usr/include/bits/wchar.h
direntry.o: /usr/include/stdio.h /usr/include/features.h
direntry.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
direntry.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
direntry.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
direntry.o: /usr/include/libio.h /usr/include/_G_config.h
direntry.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
direntry.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
direntry.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
direntry.o: /usr/include/endian.h /usr/include/bits/endian.h
direntry.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
direntry.o: /usr/include/sys/types.h /usr/include/time.h
direntry.o: /usr/include/sys/select.h /usr/include/bits/select.h
direntry.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
direntry.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
direntry.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
direntry.o: /usr/include/string.h /usr/include/xlocale.h direntry.h
directory.o: /usr/include/stdio.h /usr/include/features.h
directory.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
directory.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
directory.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
directory.o: /usr/include/libio.h /usr/include/_G_config.h
directory.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
directory.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
directory.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
directory.o: /usr/include/endian.h /usr/include/bits/endian.h
directory.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
directory.o: /usr/include/sys/types.h /usr/include/time.h
directory.o: /usr/include/sys/select.h /usr/include/bits/select.h
directory.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
directory.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
directory.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
directory.o: /usr/include/string.h /usr/include/xlocale.h directory.h
directory.o: direntry.h
disk.o: /usr/include/sys/types.h /usr/include/features.h
disk.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
disk.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
disk.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
disk.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
disk.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
disk.o: /usr/include/sys/select.h /usr/include/bits/select.h
disk.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
disk.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
disk.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
disk.o: /usr/include/bits/environments.h /usr/include/bits/confname.h
disk.o: /usr/include/getopt.h /usr/include/string.h /usr/include/xlocale.h
disk.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
disk.o: /usr/include/bits/fcntl-linux.h /usr/include/bits/stat.h
disk.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
disk.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
disk.o: /usr/include/bits/sys_errlist.h /usr/include/sys/stat.h
disk.o: /usr/include/sys/mman.h /usr/include/bits/mman.h
disk.o: /usr/include/bits/mman-linux.h disk.h /usr/include/stdint.h
disk.o: /usr/include/bits/wchar.h
fat12.o: /usr/include/stdio.h /usr/include/features.h
fat12.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
fat12.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
fat12.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
fat12.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
fat12.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fat12.o: fat12.h
fat32.o: /usr/include/stdio.h /usr/include/features.h
fat32.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
fat32.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
fat32.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
fat32.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
fat32.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fat32.o: fat32.h fat12.h
