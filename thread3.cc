#include <assert.h>
#include <iostream>
#include <thread>
#include <mutex>	// for std::mutex
#include <vector>

std::mutex stdout_lock;

void foo(int tid)
{
	stdout_lock.lock();
	std::cout << "Thread " << tid << " starting" << std::endl;
	stdout_lock.unlock();

    for (int i = 0; i < 20; ++i) 
	{
		stdout_lock.lock();
		std::cout << "Hello from thread " << tid << " i=" << i << std::endl;
		stdout_lock.unlock();
	}

	stdout_lock.lock();
	std::cout << "Thread " << tid << " ending" << std::endl;
	stdout_lock.unlock();
}
 
int main(int argc, char **argv)
{
    // Print the number of cores in the CPU that is running this program
    std::cout << std::thread::hardware_concurrency() << " concurrent threads supported." << std::endl;

    assert(argc==2 && "missing arg");   // lazy checking of cli arg count

    std::vector<std::thread*> threads;
    int num = std::stoi(argv[1]);

    for (int i=0; i<num; ++i)
        threads.push_back(new std::thread(foo, i));

    // At this point, there are actually THREE threads running in this application!

    for (int i=0; i<num; ++i)
    {
        threads.at(i)->join();
        delete threads.at(i);
    }

    std::cout << "main() exiting" << std::endl;

    return 0;
}
