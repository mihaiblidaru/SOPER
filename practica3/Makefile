# Práctica 2
# Sistemas Operativos - 2262
# 
# Pareja 9:
#        Lucia Fuentes
#        Mihai Blidaru
# 

CC = gcc
EXE = ejercicio2 ejercicio2_solved ejercicio3 ejercicio4 cadena_montaje
FLAGS = -Wall -g

$(mkdir doc)

all: $(EXE)

ejercicio2: ejercicio2.o semaforos.o
	$(CC) $(FLAGS) $^ -o $@
	
ejercicio2_solved: ejercicio2_solved.o semaforos.o
	$(CC) $(FLAGS) $^ -o $@
	
ejercicio3: ejercicio3.o semaforos.o
	$(CC) $(FLAGS) $^ -o $@
	
ejercicio4: ejercicio4.o 
	$(CC) $(FLAGS) $^ -o $@ -pthread -lm
	
cadena_montaje: cadena_montaje.o
	$(CC) $(FLAGS) $^ -o $@
	
proceso1: proceso1.c
	$(CC) $(FLAGS) $< -o $@
	
proceso2: proceso2.c
	$(CC) $(FLAGS) $< -o $@

semaforos.o: semaforos.c semaforos.h
	$(CC) -c $(FLAGS) $< -o $@

ejercicio2.o: ejercicio2.c
	$(CC) -c $(FLAGS) $< -o $@
	
ejercicio2_solved.o: ejercicio2_solved.c
	$(CC) -c $(FLAGS) $< -o $@
	
ejercicio3.o: ejercicio3.c
	$(CC) -c $(FLAGS) $< -o $@
	
ejercicio4.o: ejercicio4.c
	$(CC) -c $(FLAGS) $< -o $@ 
	
cadena_montaje.o: cadena_montaje.c
	$(CC) -c $(FLAGS) $< -o $@ 

freem:
	ipcs -m | grep 0x | cut -d" " -f 2 > /tmp/active_mem_segments.txt
	python -c "print('A'*1)";
	

PHONY: clean

clean:
	rm -f $(EXE) *.o
	rm -r -f doc/
	rm -f *.txt
	
doxygen:
	rm -f -r doc/
	mkdir doc
	doxygen Doxyfile
