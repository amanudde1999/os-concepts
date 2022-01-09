/**
    Assignment 2: Threads and Synchronization
	Name: Amro Amanuddein
	StudentID: 1572498
	Course: CMPUT 379 Fall 2021
*/

#include <iostream>
#include <string>
#include <queue>
#include <semaphore.h>  
#include <pthread.h>
#include <fstream> 
#include <sys/time.h>
#include <iomanip>
#include <algorithm>
#include "tands.cpp"
#include "helper.cpp"


// Producer function
void *producer(void *arg){
    // if we have CTRLD break while loop - file auto enters ctrl-D
	std::string line;
	while(flag){	
		sem_wait(&input);
		if (!getline(std::cin, line)){
			flag = 0;
			sem_post(&sem_current_size);
		}

		else{
			sem_post(&input);
			if (line[0] == 'T'){
				// Decrement since buffer's capacity decreases
				sem_wait(&sem_max);
				// Restrict buffer access when adding T operation
				sem_wait(&buffer_access);
				buffer.push(line);
				sem_post(&buffer_access);
				
				// Writing to file and incrementing counter, prevent file access while writing to file and incrementing
				sem_wait(&file_access);
				work++;
				int sem_val;
				sem_getvalue(&sem_current_size, &sem_val);
				gettimeofday(&end, 0);
				long seconds = end.tv_sec - begin.tv_sec;
				long microseconds = end.tv_usec - begin.tv_usec;
				double duration = seconds + microseconds*1e-6;
				output_file << duration <<" ID= 0 Q= "+std::to_string(sem_val)+" Work		"+line.substr(1,3) << std::endl;
				sem_post(&file_access);		
				// T operation was just added, increase buffer size
				sem_post(&sem_current_size);
			}
			if (line[0] == 'S'){
				sem_wait(&file_access);
				gettimeofday(&end, 0);
				long seconds = end.tv_sec - begin.tv_sec;
				long microseconds = end.tv_usec - begin.tv_usec;
				double duration = seconds + microseconds*1e-6;
				output_file << duration <<" ID= 0		 Sleep		"+line.substr(1,3) << std::endl;
				sleep++;
				sem_post(&file_access);		
				// Dont allow producer to send any Work until Sleep is done	
				sem_wait(&input);
				Sleep(std::stoi(line.substr(1,3)));
				sem_post(&input);

			}
		}
	}
	sem_wait(&file_access);
 	// double duration = (double)(clock() - start ) / (double) CLOCKS_PER_SEC;
	gettimeofday(&end, 0);
	long seconds = end.tv_sec - begin.tv_sec;
	long microseconds = end.tv_usec - begin.tv_usec;
	double duration = seconds + microseconds*1e-6;
	output_file << duration << " ID= 0		 End" << std::endl;
	sem_post(&file_access);
	return 0;
}

// Consumer function
void *consumer(void *arg){
	// OR since if buffer is not empty and CTRL-D is detected consumers still have work to do
	int thread_work = 0;
	while (buffer.size() > 0 || flag){
		// Ensure all threads do equal amount of work.
		if (*std::max_element(thread_works,thread_works+num_threads) == thread_work && isEqual(thread_work) == 0){
			continue;
		}
		// Writing to file and incrementing counter, disallow file access
		sem_wait(&file_access);
		gettimeofday(&end, 0);
		long seconds = end.tv_sec - begin.tv_sec;
		long microseconds = end.tv_usec - begin.tv_usec;
		double duration = seconds + microseconds*1e-6;
		// Consumer asks for work here since the consumer function is running
		output_file << duration <<" ID= "+std::to_string(*((int *) arg ))+	"		 Ask" << std::endl;
		ask++;
		sem_post(&file_access);

		sem_wait(&sem_current_size);
		sem_wait(&buffer_access);
			
		// Wait until something is in buffer, i.e. sem_current_size > 0 (Ask for work doesnt guarantee work)
		if (buffer.size() > 0){
			// Extract process from buffer
			std::string process = buffer.front();
			buffer.pop();
			sem_post(&buffer_access);
			// Using the semaphore for incrementation and file writing here is not neccassary
			sem_wait(&file_access);
			int sem_val;
			sem_getvalue(&sem_current_size, &sem_val);
 			// double duration = (double)(clock() - start ) / (double) CLOCKS_PER_SEC;
			gettimeofday(&end, 0);
			long seconds = end.tv_sec - begin.tv_sec;
			long microseconds = end.tv_usec - begin.tv_usec;
			double duration = seconds + microseconds*1e-6;
			output_file << duration << " ID= "+std::to_string(*((int *) arg ))+" Q= "+std::to_string(sem_val)+" Recieve	"+ process.substr(1,3)<< std::endl;
			recieve++;
			thread_work++;
			thread_works[*((int *) arg ) - 1] = thread_work;
			sem_post(&file_access);
			if (process[0] == 'T'){
				Trans(std::stoi(process.substr(1,3)));
				sem_wait(&file_access);
			 	// double duration = (double)(clock() - start ) / (double) CLOCKS_PER_SEC;
				gettimeofday(&end, 0);
				long seconds = end.tv_sec - begin.tv_sec;
				long microseconds = end.tv_usec - begin.tv_usec;
				double duration = seconds + microseconds*1e-6;
				output_file << duration << " ID= "+std::to_string(*((int *) arg ))+	"		 Complete	"+ process.substr(1,3)<< std::endl;
				complete++;
				sem_post(&file_access);
			}
			sem_post(&sem_max);
		}
		else {
			sem_post(&sem_current_size);
			sem_post(&buffer_access);
		}
	}
	//thread_works[*((int *) arg ) - 1] = thread_work;
	return 0;
}

// Function to create the producer and all consumer threads and join them
void thread_handler(){
	pthread_t ntid;
	// Producer thread created
	pthread_create(&ntid, NULL, producer, NULL);
	pthread_t* thread_ids;
	int* thread_num; 
	thread_ids = new pthread_t[num_threads+1];
	thread_ids[0] = ntid;
	thread_num = new int[num_threads];
	thread_works = new int[num_threads];
	for (int i = 1; i < num_threads+1; i++) {
		pthread_t ntid;
		thread_num[i-1] = i;
		// Main process will spawn consumer threads
		pthread_create(&ntid, NULL, consumer, &thread_num[i-1]);
		thread_ids[i] = ntid;
	}
	// If one of the threads are still running, this will not end
	for (int i = 0; i < num_threads+1; i++){
		// Ensure every thread has finished executing
		pthread_join(thread_ids[i],NULL);
	}
}


int main(int argc, char *argv[]){
	gettimeofday(&begin, NULL);
	std::string file_name;
	if (argc == 3 && std::stoi(argv[2]) != 0){
		file_name = "prodcon."+std::string(argv[2])+".log";
	}
	else{
		file_name = "prodcon.log";
	}
	output_file.open(file_name, std::ios::out);
	output_file << std::fixed << std::setprecision(3);
	num_threads = std::stoi(argv[1]);

	sem_intialization();
	thread_handler();
	summary();
	output_file.close();
	sem_destruction();

}
