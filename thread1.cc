#include <iostream>
#include <thread>
 
void foo(int tid)
{
	std::cout << "Thread " << tid << " starting" << std::endl;

    for (int i = 0; i < 20; ++i) 
		std::cout << "Hello from thread " << tid << " i=" << i << std::endl;

	std::cout << "Thread " << tid << " ending" << std::endl;
}
 
int main()
{
	// Print the number of cores in the CPU that is running this program
    std::cout << std::thread::hardware_concurrency() << " concurrent threads supported." << std::endl;

	std::thread t0(foo, 0);		// start function foo() running as a thread
	std::thread t1(foo, 1);		// start function foo() running as a thread

	// At this point, there are actually THREE threads running in this application!

    t0.join();					// wait until thread t0 is finished
    t1.join();					// wait until thread t1 is finished

	std::cout << "main() exiting" << std::endl;

	return 0;
}
