#include <assert.h>
#include <iostream>
#include <thread>
#include <mutex>	// for std::mutex
#include <vector>


std::mutex stdout_lock;

std::mutex counter_lock;
volatile int counter = 100000;

std::vector<int> tcount;

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

			++tcount[tid];		// count the number of work units that this thread has done
#if 0
			stdout_lock.lock();
			std::cout << "thread " << tid << " c=" << count_copy << std::endl;
			stdout_lock.unlock();
#endif
		}
	}


	stdout_lock.lock();
	std::cout << "Thread " << tid << " ending" << " tcount=" << tcount[tid] << " count=" << counter << std::endl;
	stdout_lock.unlock();
}
 
int main(int argc, char **argv)
{
	// Print the number of cores in the CPU that is running this program
    std::cout << std::thread::hardware_concurrency() << " concurrent threads supported." << std::endl;

	assert(argc==2 && "missing arg");   // lazy checking of cli arg count

	std::vector<std::thread*> threads;
	int num = std::stoi(argv[1]);

	tcount.resize(num, 0);	// make the tcount vector have one element for each thread & initialize them to 0

	for (int i=0; i<num; ++i)
		threads.push_back(new std::thread(foo, i));

	int total_work = 0;
	for (int i=0; i<num; ++i)
	{
		threads.at(i)->join();
		delete threads.at(i);
		total_work += tcount.at(i);
	}

	std::cout << "main() exiting, total_work=" << total_work << std::endl;

	return 0;
}
