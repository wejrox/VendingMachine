SOURCES=vm.c vm_menu.c vm_options.c vm_stock.c vm_coin.c utility.c
HEADERS=vm.h vm_menu.h vm_options.h vm_stock.h vm_coin.h utility.h vm_system.h
OBJECTS=vm.o vm_menu.o vm_options.o vm_stock.o vm_coin.o utility.o
PROGRAM=vm
DEBUG=-g
FLAGS=-ansi -pedantic -Wall $(DEBUG)
USER=S3540510

all: $(OBJECTS)
	gcc $(FLAGS) -o $(PROGRAM) $(OBJECTS)

vm.o: vm.c vm.h vm_menu.h vm_options.h vm_stock.h vm_coin.h vm_system.h utility.h
	gcc $(FLAGS) -c vm.c

vm_menu.o: vm_menu.c vm_menu.h vm_options.h vm_stock.h vm_coin.h vm_system.h utility.h
	gcc $(FLAGS) -c vm_menu.c

vm_options.o: vm_options.c vm_options.h vm_stock.h vm_coin.h vm_system.h utility.h
	gcc $(FLAGS) -c vm_options.c

vm_stock.o: vm_stock.c vm_stock.h vm_coin.h vm_system.h utility.h
	gcc $(FLAGS) -c vm_stock.c

vm_coin.o: vm_coin.c vm_coin.h vm_system.h utility.h
	gcc $(FLAGS) -c vm_coin.c

vm_utility.o: utility.c utility.h
	gcc $(FLAGS) -c utility.c

clean:
	rm -f *.o $(PROGRAM)

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) Makefile
