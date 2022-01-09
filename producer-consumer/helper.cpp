#include "helper.h"
#include <fstream>
#include <sys/time.h>

// Function to print summary information
void summary(){
	output_file << "Summary: " << std::endl;
	output_file << "	Work 		" << work << std::endl;
	output_file << "	Ask			" << ask << std::endl;
	output_file << "	Recieve		" << recieve << std::endl;
	output_file << " 	Complete	" << complete << std::endl;
	output_file << "	Sleep		" << sleep << std::endl;
	for (int i = 1; i < (num_threads+1); i++){
		output_file << "	Thread  "+std::to_string(i)+": 	"+std::to_string(thread_works[i-1]) << std::endl;
	}
	gettimeofday(&end, 0);
	long seconds = end.tv_sec - begin.tv_sec;
	long microseconds = end.tv_usec - begin.tv_usec;
	double duration = seconds + microseconds*1e-6;
	output_file << "Transactions per second: " << work/duration << std::endl;

}

// This function checks if all threads have done equal work or not, returns 1 if they have 0 otherwise
bool isEqual(int thread_work){
	for (int i = 0; i < num_threads; i++){
		if(thread_works[i] != thread_work){
			return 0;
		}
	}
	return 1;
}

// Function to initialize all semaphores
void sem_intialization(){
	// Counting Semaphores initialized
	sem_init(&sem_max, 0, 2*num_threads);
	sem_init(&sem_current_size, 0, 0);
	// Binary Semaphore - start at 1 since buffer/file are available at start
	sem_init(&buffer_access, 0, 1);
	sem_init(&file_access, 0, 1);
	sem_init(&input, 0, 1);
}

// Function to destroy all semaphores
void sem_destruction(){
	sem_destroy(&sem_max);
	sem_destroy(&sem_current_size);
	sem_destroy(&buffer_access);
	sem_destroy(&file_access);
	sem_destroy(&input);
}


