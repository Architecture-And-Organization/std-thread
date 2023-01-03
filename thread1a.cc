#include <assert.h>
#include <iostream>
#include <thread>
#include <vector>
 
void foo(int tid)
{
	std::cout << "Thread " << tid << " starting" << std::endl;

    for (int i = 0; i < 20; ++i) 
		std::cout << "Hello from thread " << tid << " i=" << i << std::endl;

	std::cout << "Thread " << tid << " ending" << std::endl;
}
 
int main(int argc, char **argv)
{
	// Print the number of cores in the CPU that is running this program
    std::cout << std::thread::hardware_concurrency() << " concurrent threads supported." << std::endl;

	assert(argc==2 && "missing arg");	// lazy checking of cli arg count

	std::vector<std::thread*> threads;
	int num = std::stoi(argv[1]);

	for (int i=0; i<num; ++i)
		threads.push_back(new std::thread(foo, i));

	// At this point, there are actually num+1 threads running in this application!

	for (int i=0; i<num; ++i)
	{
		threads.at(i)->join();
		delete threads.at(i);
	}

	std::cout << "main() exiting" << std::endl;

	return 0;
}
