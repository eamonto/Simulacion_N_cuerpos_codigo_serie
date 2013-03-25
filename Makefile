# PREDEFINED VARIABLES
CFLAGS = -O3 -Wall -I. -Wno-unused -fopenmp
LFLAGS = -lm
CC = gcc

#GLOBAL VARIABLES
INITFILE = init.txt
INPUTFILE = entrada.dat
OUTPUTFILE = salida

# MODULES
MODULES = lib_simulacion integradores
MODULESC = $(MODULES:=.c)
MODULESH = $(MODULES:=.h)
MODULESO = $(MODULES:=.o)
PROGRAM = simulacion

# LINKING
$(PROGRAM).out: $(PROGRAM).o $(MODULESO)
	$(CC) $^ -o $@ $(LFLAGS) $(CFLAGS)
	rm -rf $(MODULESO) $(PROGRAM).o

# MODULES' RULES
$(MODULES): $(MODULESC) $(MODULESH)
	$(CC) $(CFLAGS) $@.c

# PROGRAM COMPILATION
$(PROGRAM).o: $(PROGRAM).c
	$(CC) -c $(CFLAGS) $<

run:
	./$(PROGRAM).out $(INITFILE) $(INPUTFILE) $(OUTPUTFILE) 

clean:
	rm -rf *~ *.out *.o *# *.log

all: $(PROGRAM).out run


