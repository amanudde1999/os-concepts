#ifndef HELPER_H
#define HELPER_H

#include <fstream>
#include <sys/time.h>
#include <queue>
#include <pthread.h>
#include <semaphore.h>

// Define global variables here
bool flag = 1;
int num_threads;
int sleep = 0;
int work = 0;
int recieve = 0;
int complete = 0;
int ask = 0;
int *thread_works;
std::fstream output_file;
std::queue<std::string> buffer;
struct timeval begin, end;

// Define semaphores here
sem_t sem_max;
sem_t sem_current_size;
sem_t buffer_access;
sem_t file_access;
sem_t input;

void summary();	
void sem_destruction();
void sem_intialization();
void isEqual();

#endif
