CC=g++
CCX=mpic++

CFLAG_PAR=-D_PARALLEL
CFLAG_SEC=-D_SERIE
SRC=pre-proc.cpp

par:
	$(CCX) $(CFLAG_PAR) $(SRC) -o $@

sec:
	$(CC) $(CFLAG_SEC) $(SRC) -o $@

run_par:
	mpirun -np 4 ./par

run_sec:
	./sec

clean:
	rm par sec
