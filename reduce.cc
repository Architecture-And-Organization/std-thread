#include <assert.h>
#include <iostream>
#include <thread>
#include <mutex>	// for std::mutex
#include <vector>

constexpr int rows = 1000;
constexpr int cols = 100;

std::mutex stdout_lock;

std::mutex counter_lock;
volatile int counter = rows;

std::vector<int> tcount;
std::vector<uint64_t> sum;
int work[rows][cols];

void foo(int tid)
{
	stdout_lock.lock();
	std::cout << "Thread " << tid << " starting" << std::endl;
	stdout_lock.unlock();

	bool done = false;
	while (!done)
	{
		int count_copy;

		counter_lock.lock();
		{
			if (counter>0)
				--counter;
			else
				done = true;
			count_copy = counter;		// grab a copy that we know is consistent w/done flag!
		}
		counter_lock.unlock();

		if (!done)
		{
			// do work using the value of count_copy to determine what to do!
			(void)count_copy;	// remove compiler warning

			// add up all cols for the row with the index 'count_copy'
			for (int i=0; i<cols; ++i)
				sum[tid] += work[count_copy][i];

			++tcount[tid];		// count the number of work units that this thread has done
#if 0
			stdout_lock.lock();
			std::cout << "thread " << tid << " c=" << count_copy << std::endl;
			stdout_lock.unlock();
#endif
		}
	}


	stdout_lock.lock();
	std::cout << "Thread " << tid << " ending" << " tcount=" << tcount[tid] << " count=" << counter << " sum=" << sum[tid] << std::endl;
	stdout_lock.unlock();
}
 
int main(int argc, char **argv)
{
	// Print the number of cores in the CPU that is running this program
    std::cout << std::thread::hardware_concurrency() << " concurrent threads supported." << std::endl;

	assert(argc==2 && "missing args");   // lazy checking of cli arg count

	int num = std::stoi(argv[1]);
	tcount.resize(num, 0);	// make the tcount vector have one element for each thread & initialize them to 0
	sum.resize(num, 0);		// need a place to sum elements for each thread

	// fill the 2-D work matrix with random numbers
	for (int r=0; r<rows; ++r)
		for (int c=0; c<cols; ++c)
			//work[r][c] = 1; //rand();
			//work[r][c] = rand() % 1000;
			work[r][c] = rand();

	std::vector<std::thread*> threads;
	for (int i=0; i<num; ++i)
		threads.push_back(new std::thread(foo, i));

	int total_work = 0;
	uint64_t gross_sum = 0;
	for (int i=0; i<num; ++i)
	{
		threads.at(i)->join();
		delete threads.at(i);
		total_work += tcount.at(i);
		gross_sum += sum.at(i);
	}

	std::cout << "main() exiting, total_work=" << total_work << " gross_sum=" << gross_sum << std::endl;

	return 0;
}
