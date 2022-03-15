mpicc -o master ./master.c; mpicc -o slave ./slave.c; mpirun -np 1 : ./master -np 3 slave
