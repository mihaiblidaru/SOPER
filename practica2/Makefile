# Práctica 2
# Sistemas Operativos - 2262
# 
# Pareja 9:
#        Lucia Fuentes
#        Mihai Blidaru
# 

CC = gcc
EXE = ejercicio2 ejercicio3 ejercicio4 ejercicio6a ejercicio6b ejercicio8 ejercicio9
FLAGS = -Wall -g 

$(mkdir doc)

all: $(EXE)

ejercicio2: ejercicio2.c
	$(CC) $(FLAGS) $< -o $@

ejercicio3: ejercicio3.c
	$(CC) $(FLAGS) $< -o $@

ejercicio4: ejercicio4.c
	$(CC) $(FLAGS) $< -o $@

ejercicio6a: ejercicio6a.c
	$(CC) $(FLAGS) $< -o $@

ejercicio6b: ejercicio6b.c
	$(CC) $(FLAGS) $< -o $@
	
ejercicio8: ejercicio8.c
	$(CC) $(FLAGS) $< -o $@
	
ejercicio9: ejercicio9.c
	$(CC) $(FLAGS) $< -o $@

semaforos.o: semaforos.c semaforos.h
	$(CC) -c $(FLAGS) -o $@

PHONY: clean

clean:
	rm -f $(EXE) *.o
	rm -r -f doc/
	
doxygen:
	rm -f -r doc/
	mkdir doc
	doxygen Doxyfile
