##############################################################################
# COSC1283/1284 - Programming Techniques
# Semester 2 2011 Assignment #2
# Full Name        : Alyssa Biasi
# Course Code      : COSC1284
# Program Code     : BP215
##############################################################################


CFLAGS = -Wall -ansi -pedantic
HFILES = vm_options.h vm_utility.h vm.h
CFILES = vm.c vm_utility.c vm_options.c structFunctions.c
OFILES = vm.o vm_utility.o vm_options.o structFunctions.o

all: structFunctions.o vm_options.o vm_utility.o vm.o
	gcc $(CFLAGS) -gstabs $(OFILES) -o vm

structFunctions.o: structFunctions.c $(HFILES)
	gcc $(CFLAGS) -c structFunctions.c

vm_options.o: vm_options.c $(HFILES)
	gcc $(CFLAGS) -c vm_options.c

vm_utility.o: vm_utility.c $(HFILES)
	gcc $(CFLAGS) -c vm_utility.c

vm.o: vm.c $(HFILES)
	gcc $(CFLAGS) -c vm.c

clean:
	rm -rf *.o vm core

archive:
	zip vm.zip $(CFILES) $(HFILES) makefile README.txt
