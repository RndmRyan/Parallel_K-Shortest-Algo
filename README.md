
# Parallelized top K Shortest Algorithm using OpenMP and MPI

This project used the Doctor Who dataset, from: [Doctor Who Dataset](https://github.com/manuel-dileo/doctor-who-dataset/tree/main) 


For this project, we started in C but our code unfortunately had very little memory management, causing the whole program to crash because of the huge dataset. We then shifted to C++, to use libraries such as Vectors, for 10x better memory management without the need of micromanaging our own code. For this we used the code provided on GeeksforGeeks as mentioned in the references.

## Stage 0:
Firstly, we wrote the code using a loop for all 10 sets and timed a few runs of the serial code. Then we proceeded to add Parallelization in stages.

## Stage 1:
For MPI, our initial approach was to use Dockers with 10 clusters, 1 for each pair of start and end nodes. However, even with 24Gb RAM, our computer was unable to run 10 clusters. So we changed our approach to create only 2 clusters, with 10 processes in total. The only few lines of code changed or added was in main(), and to run this the following commands were used:

_Run Command in the Docker File:_
```
run mpic++ -o p project.cpp -std=c++11
```
_Execute Commands:_
```
./cluster.sh up size=2
./cluster.sh login
mpirun -np 10 ./p
```

## Stage 2:
Our next step was to implement OpenMP. The changes in our code happened in the findKShortestPaths function. The loops were assigned to run in parallel. However, some lines were marked as critical segments so that they can be read/updated in serial, to not fall in any syncronization issues. 

_Change Run Command in the Docker File:_
```
run mpic++ -o p project.cpp -std=c++11 -fopenmp
```
_Execute command:_
```
Exec mpirun -np 10 ./p
```

| Runs | Serial | MPI | MPI + OpenMP |
| :-----: | :---: | :---: | :---: |
| 1 | 28 | 6 | 3 |
| 2 | 7 | 4 | 6 |
| 3 | 15 | 6 | 3 |
| 4 | 5 | 3 | 5 |
| 5 | 20 | 4 | 4 |
| Avg | 15 | 4.6 | 4.2 |

Here is our analytical finding on our serial program against the program with MPI and Openmp implementation.

The serial program has the worst runtime due to the fact that the program is running on a single core, and the MPI program is faster because we divided the code execution among 2 clusters, with 5 processes each. This had a significant improvement on the total time for the code to run. To further make this more efficient we then allowed the loops to run in parallel. This did reduce the average execution time somewhat, but it  also shows that the loops were not that CPU intensive.


Here is how we calculated the speedup:

**Speedup Formula = Serial Execution Time / Parallel Execution Time**

With MPI = Average Serial Time / Average MPI Time

With MPI + OpenMP = Average Serial Time / Average MPI+OpenMP Time

**Speedup with MPI:** 3.261

**Speedup with MPI & OpenMP:** 3.571


_References:_
1. [1st to Kth shortest path lengths from node 1 to N in given Graph - GeeksforGeeks](https://www.geeksforgeeks.org/1st-to-kth-shortest-path-lengths-from-node-1-to-n-in-given-graph/)
2. [Path from a given source to a given destination having Kth largest weight in a Graph - GeeksforGeeks](https://www.geeksforgeeks.org/path-from-a-given-source-to-a-given-destination-having-kth-largest-weight-in-a-graph/)
