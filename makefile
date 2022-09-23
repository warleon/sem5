p1:
	mpic++ p1.cpp -o p1 -Wall
	mpirun p1
p1_pogaso:
	mpic++ p1_pogaso.cpp -o p1_pogaso -Wall
	mpirun --use-hwthread-cpus p1_pogaso
p2:
	mpic++ p2.cpp -o p2
	mpirun p2
p3:
	mpic++ p3.cpp -o p3
	mpirun p3

.PHONY: p1 p1_pogaso p2 p3