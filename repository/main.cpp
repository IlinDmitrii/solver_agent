#include <iostream>
#include <fstream>
#include <string>

#include <windows.h>

//#include <chrono>
//#include <thread>
#include "mpi.h"

int input(const char* fn, int*& serial_num, int*& cores, size_t* num_of_tasks)
{
	std::string str;
	std::ifstream finput(fn, std::ios::in);
	if (!finput)
	{
		std::cout << "Can't open input file\n";
		return -1;
	}
	while (getline(finput, str))
	{
		(*num_of_tasks)++;
	}
	finput.clear();
	finput.seekg(0);
	cores = new int[*num_of_tasks];
	serial_num = new int[*num_of_tasks];
	for (int i = 0; i < *num_of_tasks; i++) {
		finput >> serial_num[i];
		finput >> cores[i];
	}
	finput.close();
	return 0;
}


int sort_tasks(int* serial_num, int* procs, int**& how_to_run, const int& numprocs, const size_t& num_of_tasks)
{
	int number_of_starts = 0; // total number of starts
	int* remeining_procs = new int[num_of_tasks]; // remeining procs in each start (num_of_tasks = max of number of starts)
	int* start_to_run = new int[num_of_tasks];
	for (int i = 0; i < num_of_tasks; i++) {
		int j;

		int min = numprocs + 1; //min of remeining procs in all starts 
		int seq_num = 0; //the sequence number of the start with the minimum number of processors remaining

		for (j = 0; j < number_of_starts; j++) {
			if (remeining_procs[j] >= procs[i] && remeining_procs[j] - procs[i] < min) {
				seq_num = j;
				min = remeining_procs[j] - procs[i];
			}
		}
		if (min == numprocs + 1) {
			remeining_procs[number_of_starts] = numprocs - procs[i];
			start_to_run[i] = number_of_starts;
			number_of_starts++;
		}
		else
		{
			remeining_procs[seq_num] -= procs[i];
			start_to_run[i] = seq_num;
		}
	}
	delete[] remeining_procs;

	how_to_run = new int* [number_of_starts];
	for (int i = 0; i < number_of_starts; i++) how_to_run[i] = new int[numprocs];
	int* freeprocs = new int[number_of_starts];
	for (int i = 0; i < number_of_starts; i++) { freeprocs[i] = numprocs; }
	for (int i = 0; i < num_of_tasks; i++)
	{
		for (int j = numprocs - freeprocs[start_to_run[i]]; j < numprocs - freeprocs[start_to_run[i]] + procs[i]; j++) { how_to_run[start_to_run[i]][j] = serial_num[i]; }
		freeprocs[start_to_run[i]] -= procs[i];
	}
	for (int i = 0; i < number_of_starts; i++) { for (int j = numprocs - freeprocs[i]; j < numprocs; j++) { how_to_run[i][j] = -1; } }
	delete[] start_to_run;
	delete[] freeprocs;

	return number_of_starts;
}

int main(int argc, char** argv)
{
	//int nnumprocs = int((argv)[2][0]) - int('0'); // number of procs (*) from mpiexec -n * - ������
	int numprocs; // number of procs from MPI_Comm_size
	int i, j;
	int err = 0;
	int myid; // id of each proc
	int num_of_starts; // total number of starts
	size_t num_of_tasks = 0; //total number of tasks
	int* serial_num; //array of (serial number of task)
	int* procs; //array of (procs per task)
	int* start_to_run; // on which processor to run
	err = input("table.txt", serial_num, procs, &num_of_tasks);
	if (err) return -1;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	int** how_to_run;
	num_of_starts = sort_tasks(serial_num, procs, how_to_run, numprocs, num_of_tasks);

	for (i = 0; i < num_of_starts; i++)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		if (how_to_run[i][myid] != -1)
		{
			std::cout << "the task " << how_to_run[i][myid] << " started on the processor " << myid << " \n";
;
		}
		//Sleep(100);
		if (how_to_run[i][myid] != -1)
		{
			std::cout << "the task " << how_to_run[i][myid] << " has finished running on the processor " << myid << " \n";
		}
		if (myid == 0) std::cout << "----------------------------------------------------------------------------------------------------------------------\n";
	}
	MPI_Finalize();

	delete[] procs;
	delete[] serial_num;
	delete[] how_to_run;
	return 0;
}
