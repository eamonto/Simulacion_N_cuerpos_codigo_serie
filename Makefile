# PREDEFINED VARIABLES
CFLAGS = -c -O3 -Wall -I. -Wno-unused
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
	$(CC) $^ -o $@ $(LFLAGS)
	rm -rf $(MODULESO) $(PROGRAM).o

# MODULES' RULES
$(MODULES): $(MODULESC) $(MODULESH)
	$(CC) $(CFLAGS) $@.c -o $@.o

# PROGRAM COMPILATION
$(PROGRAM).o: $(PROGRAM).c $(MODULESH)
	$(CC) $(CFLAGS) $< -o $@

run:
	./$(PROGRAM).out $(INITFILE) $(INPUTFILE) $(OUTPUTFILE) 

clean:
	rm -rf *~ *.out *.o *# *.log

all: $(PROGRAM).out run


