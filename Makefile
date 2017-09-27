#! /bin/bash
SOURCES="vm.c vm_menu.c vm_options.c vm_stock.c vm_coin.c utility.c"
HEADERS="vm.h vm_menu.h vm_options.h vm_stock.h vm_coin.h utility.h vm_system.h"
PROGRAM="vm"
DEBUG="-g"
FLAGS="-ansi -pedantic -Wall $DEBUG"
USER="S3540510"

if [[ $1 == "-b" ]]
then
	gcc $FLAGS -o $PROGRAM $SOURCES
elif [[ $1 == "-e" ]]
then
	./$PROGRAM stock.dat coins.dat
elif [[ $1 == "-be" ]]
then
	gcc $FLAGS -o $PROGRAM $SOURCES
	./$PROGRAM stock.dat coins.dat
elif [[ $1 == "-v" ]]
then
	valgrind --leak-check=full --track-origins=yes -v ./vm stock.dat coins.dat
elif [[ $1 == "-g" ]]
then
	gdb --args vm stock.dat coins.dat
elif [[ $1 == "-clean" ]]
then
	rm $PROGRAM
elif [[ $1 == "-f" ]]
then
	zip $USER-a2 $SOURCES $HEADERS Makefile
else
	echo "No arguments specified."
	echo "-b = Build               | -e = Execute           | -be = Build and Execute"
	echo "-v = Valgrind Leak-Check | -g = GDB Check"
	echo "-clean = Cleanup         | -f = Final Compression"
	exit
fi