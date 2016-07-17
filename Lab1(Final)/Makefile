# Makefile for GPIO User Space

SOURCES= creativeSong.c
EXECUTABLE = creativeSong

# target used to transfer files to the beaglebone interface.
transfer:
	scp creativeSong.c root@192.168.7.2:~
	scp creativeSong.h root@192.168.7.2:~
	scp Makefile root@192.168.7.2:~
# target used to create executable files from the c file.
create: creativeSong.c creativeSong.h
	gcc -o $(EXECUTABLE) creativeSong.c 
# target used to execute the executable file.
run: 
	./a.out

# target used to stop the cross-compile
stop:
	arm-linux-gnueabihf-gcc -o creativeSong creativeSong.c

# target used to delete the executable files
clean:
	
	@ echo Cleaning...	
	rm -rf *.o $(EXECUTABLE)





