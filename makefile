p1_clock:
	mpic++ p1_clock.cpp -o p1_clock -Wall
	mpirun --use-hwthread-cpus p1_clock
p1_mod:
	mpic++ p1_mod.cpp -o p1_mod -Wall
	mpirun --use-hwthread-cpus p1_mod
p2_block:
	mpic++ p2_block.cpp -o p2_block
	mpirun --use-hwthread-cpus p2_block
p2_nonblock:
	mpic++ p2_nonblock.cpp -o p2_nonblock
	mpirun --use-hwthread-cpus p2_nonblock
p3:
	mpic++ p3.cpp -o p3
	mpirun p3

.PHONY: p1_clock p1_mod p2_block p2_nonblock p3