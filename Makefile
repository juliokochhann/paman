build_dir = build
src_dir   = src
objects   = $(build_dir)/paman.o $(build_dir)/main.o
cc        = gcc
cflags    = -Wall -std=c99

paman : $(objects)
	$(cc) $(cflags) -o $(build_dir)/paman $(objects)

$(build_dir)/paman.o : $(src_dir)/paman.c $(src_dir)/paman.h
	$(cc) $(cflags) -c $< -o $@

$(build_dir)/main.o  : $(src_dir)/main.c $(src_dir)/paman.h
	$(cc) $(cflags) -c $< -o $@

.PHONY : clean
clean :
	rm $(build_dir)/paman $(objects)

.PHONY: install
install :
	cp $(build_dir)/paman /usr/bin

.PHONY: uninstall
uninstall :
	rm /usr/bin/paman